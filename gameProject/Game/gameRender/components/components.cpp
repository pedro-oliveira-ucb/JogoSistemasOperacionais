#include "components.h"

#include <raylib/raymath.h>
#include <stdio.h>


bool components::DrawButton( Rectangle bounds , const char * label , Color color , Color textColor ) {
	DrawRectangleRec( bounds , color );

	int fontSize = 20;
	int textWidth = MeasureText( label , fontSize );
	int textHeight = fontSize; // Aproxima��o, funciona bem com fontes padr�o

	float textX = bounds.x + ( bounds.width - textWidth ) / 2.0f;
	float textY = bounds.y + ( bounds.height - textHeight ) / 2.0f;

	DrawText( label , textX , textY , fontSize , textColor );

	return CheckCollisionPointRec( GetMousePosition( ) , bounds ) && IsMouseButtonPressed( MOUSE_LEFT_BUTTON );
}


bool components::DrawSlider( Rectangle bounds , float * value , const char * label , float min , float max , Color backColor , Color frontColor ) {
	bool changed = false;

	// Draw background
	DrawRectangleRec( bounds , backColor );

	// Tamanho do "pino" (c�rculo)
	float knobRadius = 8.0f;

	// Calcular posi��o X do knob com base no valor
	float percent = ( *value - min ) / ( max - min );
	float knobX = bounds.x + percent * bounds.width;

	// Intera��o com mouse
	Vector2 mouse = GetMousePosition( );
	if ( CheckCollisionPointRec( mouse , bounds ) && IsMouseButtonDown( MOUSE_LEFT_BUTTON ) ) {
		percent = ( mouse.x - bounds.x ) / bounds.width;
		percent = Clamp( percent , 0.0f , 1.0f );
		*value = min + percent * ( max - min );
		changed = true;
	}

	// Desenhar barra preenchida
	Rectangle filled = bounds;
	filled.width = percent * bounds.width;
	DrawRectangleRec( filled , frontColor );

	// Desenhar knob (c�rculo)
	DrawCircle( ( int ) knobX , ( int ) ( bounds.y + bounds.height / 2 ) , knobRadius , RED );

	// Label alinhado � esquerda
	int fontSize = 12;
	DrawText( label , bounds.x , bounds.y - fontSize - 2 , fontSize , WHITE );

	// Valor � direita (com margem de 4px)
	char valueText[ 32 ];
	sprintf_s( valueText , sizeof( valueText ) , "%.0f" , *value );
	int textWidth = MeasureText( valueText , fontSize );
	DrawText( valueText , bounds.x + bounds.width - textWidth , bounds.y - fontSize - 2 , fontSize , WHITE );

	return changed;
}