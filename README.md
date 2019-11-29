# VisualT
This is a tiny C library (+ editor) I wrote in C when I was 16.
At the time I was in the 4th year of highschool, and we were switching to Java after a year of studying C.

This project was meant to create an environment similar to [Scratch](https://scratch.mit.edu/ "www.scratch.mit.edu"), but in C, with graphic "Objects" (*Sprites in scratch*) and "Sprites" (*Costumes in scratch*). Objects are represented by `.obj` text files which can be imported or embedded within the source.


```
VisualT is a tiny game-oriented ascii graphic library

you can compile the examples directly by adding to your command
" ../../VisualT/xx/libvisualtxx.a "
(where xx stands for 32 or 64)

Contents{
	VisualT{
		visualt.h	 	:library header
		32{
			editor32	:32bit editor executable
			libvisualt32.a	:32bit library
		}
		64{
			editor64	:64bit editor executable
			libvisualt64.a	:64bit library
		}
	}
	Examples{
		example01{...}		:basics
		example02{...}		:gamemode() and layers
		example03{...}		:animation
		example04{...}		:basic sidescrolling engine
		each example contains{
			main.c		:example source
			obj{...}	:obj used in the example
			text{...}	:additional obj text sources
		}
	}
}
```