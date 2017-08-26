//=============================================================================
// ■ tiled_map.hpp
//=============================================================================

#pragma once

namespace VM76 {

	struct TileData {
		unsigned char tid;
		unsigned char data_flag;
	};

	#define TERRIAN_MAX_HEIGHT 60


	class DataMap : public Object {
	public:
		TileData* map = NULL;
		int width, length, height;
		TileData constStone;

		thread* map_save_worker = NULL;

	public:
		DataMap(int w, int h, int l);
		~DataMap();

		inline size_t index(int x, int y, int z) {
			return width * length * y + length * z + x;
		}
		inline size_t index(glm::vec3 v) {
			return index(v.x, v.y, v.z);
		}

		inline TileData tileQuery(int x, int y, int z) {
			if (x >= 0 && x < width && y >= 0 && y < length && z >= 0 && z < height) {
				return map[index(x, y, z)];
			} else {
				return constStone;
			}
		}

		inline char tidQuery(int x, int y, int z) {
			return tileQuery(x, y, z).tid;
		}

		void save_map();
		bool read_map();

		void generate_flat();
		void generate_V1();
	};

	class TiledMap : public RenderObject {
	public:
		DataMap* map = NULL;
		int width, length, height;
		glm::vec3 mount_point = glm::vec3(0.0);
		glm::mat4 translate = glm::mat4(1.0);

		bool is_valid = false;

	private:
		Tiles* cinstance[16];

		GDrawable* obj;

	public:
		inline int calcTileIndex(int x, int y, int z) { return (width * length) * y + (length) * z + x; }
		inline int calcTileIndex(glm::vec3 pos) { return (width * length) * pos.y + (length) * pos.z + pos.x; }

		static void init_cinstances(Tiles* cinstance[]);
		static Tiles* get_instances(int id);

		TiledMap(int x, int y, int z, glm::vec3 wp, DataMap* m);
		void render();
		GDrawable* getBatch();
		void bake_tiles();
		~TiledMap();
	};

	class Map : public RenderObject {
	private:
		CmdList* cmd_buffer = NULL;

	public:
		DataMap* map = NULL;
		TiledMap** chunks = NULL;
		int width, length, height; // chunk as unit
		int bw, bl, bh; // block as unit
		int CHUNK_SIZE;

	public:
		Map(int w, int h, int l, int csize);
		void place_block(glm::vec3 dir, int tid);
		void update_render_buffer();
		void render();

		inline int calcChunkIndex(int x, int y, int z) {
			return (width * length) * y + (length) * z + x;
		}

		~Map();
	};

	class PhysicsMap : public PhyObject {
	private:
		Map* robj;
		BoxCollider** buf = NULL;

	public:
		BoxCollider** get_collide_iterator(OBB* b);

		PhysicsMap(Map* m);
		~PhysicsMap();
	};
}
