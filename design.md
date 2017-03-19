design.md
=========

- [ ] VLib: a header-only library for the VM/76 project
	- [x] VEndian
	- [x] VExplosion: end your program gracefully
	- [x] VMath
	- [x] VVector
	- [x] VRingBuffer
	- [ ] VByteStream: file/string abstraction layer
		- [ ] VByteStream_File
		- [ ] VByteStream_String
	- [x] VBinaryFileReader
	- [x] VBinaryFileWriter
- [ ] ASM76: the assembly — scripting and virtual machine
	- [ ] spec.md
		- [x] first usable draft
		- [ ] stable release 1.0
	- [ ] VM implementation
 		- [x] ASM76 bytecode translator architecture
		- [ ] Instruction sets
			- [x] 76-Basic
			- [ ] 76-Float
			- [ ] 76-Vectors
			- [ ] API calls to Game Engine
	- [x] assembler
		- [x] basic translation
		- [x] address tag
	- [x] disassembler
		- [x] basic implementation
- [ ] *(UNDEFINED NAME)*: a high-level programming language that compiles to ASM76
	- [ ] spec.md
		- [ ] design philosophy
		- [ ] first usable draft
		- [ ] stable release 1.0
	- [ ] the compiler
- [ ] VMDE: draw engine
	- [x] init_engine
	- [x] update
	- [x] engine state related
		- [x] get_frame_count
		- [x] get_fps
	- [x] Shaders
		- [x] Load & Compile shaders
		- [x] Use & Call shaders
		- [x] variables binding functions
	- [x] GDrawable
		- [x] Create drawable object
		- [x] Instanced rendering
		- [x] Draw Call
		- [x] Buffer Objects management
	- [x] Res::Texture: texture resources
		- [x] load texture
		- [x] Texture to shader binding
	- [ ] Res::AudioCache: (pre)loads an audio file
		- [ ] load function
	- [x] Audio: audio support
		- [x] Channel: a logical representation of one audio source
			- [x] Channel_Mute: outputs no sound
			- [x] Channel_Noise: outputs white noise
			- [x] Channel_Triangle
			- [x] Channel_Sine
			- [x] Channel_Vorbis
		- [x] stop
		- [x] play_channel
		- [ ] play_music
		- [ ] play_sound
- [ ] VMGS: game system
	- [x] main
	- [ ] Control & View
		- [x] Demo View
		- [x] FPS View (Free explore)
		- [x] God View
		- [ ] Physics binded view
	- [ ] Game Objects
		- [x] Cube (Tiled cube)
		- [ ] Model
		- [x] Terrian
 			- [x] Map loader & save
 			- [x] Map vertices & data baker
		- [ ] Light source
		- [ ] Entity
		- [x] object helper class
	- [ ] Physics
	- [ ] Effects
	- [ ] Game
		- [x] config.hpp
		- [ ] Game Logic