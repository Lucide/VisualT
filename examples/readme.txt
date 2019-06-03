VisualT DEMO PACK v1

VisualT is a gamemaking-oriented ascii graphic library

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

Mr. Fox
