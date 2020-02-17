#pragma once
#ifndef VISUALT_INCLUDE_VISUALT_VISUALTUNPREFIXED_H_
#define VISUALT_INCLUDE_VISUALT_VISUALTUNPREFIXED_H_

#include "visualt.h"

//----MISC----
#define about(...) vtAbout( __VA_ARGS__ )
//----INITIALIZATION----
#define initializeBlank(...) vtInitializeBlank( __VA_ARGS__ )
#define initializeArray(...) vtInitializeArray( __VA_ARGS__ )
#define initializeFile(...) vtInitializeFile( __VA_ARGS__ )
#define initializeString(...) vtInitializeString( __VA_ARGS__ )
#define initializeObj(...) vtInitializeObj( __VA_ARGS__ )
#define release(...) vtRelease( __VA_ARGS__ )
#define cloneSprite(...) vtCloneSprite( __VA_ARGS__ )
#define resize(...) vtResize( __VA_ARGS__ )
//----REFRESH----
#define render(...) vtRender( __VA_ARGS__ )
#define stamp(...) vtStamp( __VA_ARGS__ )
#define print(...) vtPrint( __VA_ARGS__ )
#define printToString(...) vtPrintToString( __VA_ARGS__ )
//----SPRITE----
#define sprites(...) vtSprites( __VA_ARGS__ )
#define spriteInd(...) vtSpriteInd( __VA_ARGS__ )
#define nextSprite(...) vtNextSprite( __VA_ARGS__ )
#define precSprite(...) vtPrecSprite( __VA_ARGS__ )
#define setSprite(...) vtSetSprite( __VA_ARGS__ )
#define width(...) vtWidth( __VA_ARGS__ )
#define height(...) vtHeight( __VA_ARGS__ )
#define setText(...) vtSetText( __VA_ARGS__ )
#define clear(...) vtClear( __VA_ARGS__ )
#define fill(...) vtFill( __VA_ARGS__ )
#define overlay(...) vtOverlay( __VA_ARGS__ )
#define printAxes(...) vtPrintAxes( __VA_ARGS__ )
//----OBJ----
#define visible(...) vtVisible( __VA_ARGS__ )
#define show(...) vtShow( __VA_ARGS__ )
#define hide(...) vtHide( __VA_ARGS__ )
#define setVisibility(...) vtSetVisibility( __VA_ARGS__ )
//----PEN----
#define penChar(...) vtPenChar( __VA_ARGS__ )
#define penSize(...) vtPenSize( __VA_ARGS__ )
#define setPenSize(...) vtSetPenSize( __VA_ARGS__ )
#define setPenChar(...) vtSetPenChar( __VA_ARGS__ )
//----MOVE----
#define xPosition(...) vtXPosition( __VA_ARGS__ )
#define yPosition(...) vtYPosition( __VA_ARGS__ )
#define gotoXY(...) vtGotoXY( __VA_ARGS__ )
#define gotoX(...) vtGotoX( __VA_ARGS__ )
#define gotoY(...) vtGotoY( __VA_ARGS__ )
#define changeX(...) vtChangeX( __VA_ARGS__ )
#define changeY(...) vtChangeY( __VA_ARGS__ )
#define align(...) vtAlign( __VA_ARGS__ )
#define isTouching(...) vtIsTouching( __VA_ARGS__ )
#define isTouchingChar(...) vtIsTouchingChar( __VA_ARGS__ )
#define isOutside(...) vtIsOutside( __VA_ARGS__ )

#endif //VISUALT_INCLUDE_VISUALT_VISUALTUNPREFIXED_H_
