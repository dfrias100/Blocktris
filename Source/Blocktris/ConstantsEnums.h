/*----------------------------------------------------------------------------------|
|  Blocktris: A block puzzle game written in C++ using SFML.                        |
|                                                                                   |
|  Copyright(C) 2022  Daniel Frias						    |
|                                                                                   |
|  This program is free software; you can redistribute it and/or		    |
|  modify it under the terms of the GNU General Public License			    |
|  as published by the Free Software Foundation; either version 2		    |
|  of the License, or (at your option) any later version.			    |
|  This program is distributed in the hope that it will be useful,		    |
|  but WITHOUT ANY WARRANTY; without even the implied warranty of		    |
|  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the			    |
|  GNU General Public License for more details.					    |
|  You should have received a copy of the GNU General Public License		    |
|  along with this program; if not, write to the Free Software			    |
|  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.  |
|-----------------------------------------------------------------------------------*/

#ifndef CONSTANTSENUMS_H
#define CONSTANTSENUMS_H

///////////////////////////////////////
// Pre-processor/constexpr constants //
///////////////////////////////////////
static constexpr float SquareSize = 35.0f;

static constexpr float BoardSizeX = SquareSize * 10.0f;
static constexpr float BoardSizeY = SquareSize * 20.0f;

static constexpr float ScreenWidth = 2.5f * BoardSizeX;
static constexpr float ScreenHeight = 800.0f;

static constexpr float BoardOffsetX = (ScreenWidth - BoardSizeX) / 2.0f;
static constexpr float BoardOffsetY = (ScreenHeight - BoardSizeY) / 2.0f;

static constexpr float PreviewRectSizeX = SquareSize * 4.0f + 15.0f;
static constexpr float PreviewRectSizeY = 3.0f * PreviewRectSizeX;
static constexpr float PreviewRectOffsetX = BoardOffsetX * 1.5f + BoardSizeX - PreviewRectSizeX * 0.5f + 2.5f;
static constexpr float PreviewRectOffsetY = 0.15f * ScreenHeight;

static constexpr float FirstDigitLineX = (48.0f / 875.0f) * ScreenWidth;
static constexpr float FirstDigitLineY = (392.0f / 800.0f) * ScreenHeight;

static constexpr float DigitLineGap = (111.0f / 800.0f) * ScreenHeight;

static constexpr float FirstMoveLineX = (669.0f / 875.0f) * ScreenWidth;
static constexpr float FirstMoveLineY = (607.0f / 800.0f) * ScreenHeight;

static constexpr float MoveSpriteVGap = (3.0f / 800.0f) * ScreenHeight;

static constexpr int MoveSpriteWidth = 163;
static constexpr int MoveSpriteHeight = 28;

static constexpr int FontWidth = 25;
static constexpr int FontHeight = 31;

static constexpr unsigned long long QUARTER_SECOND = 15;
static constexpr unsigned long long HALF_SECOND = 30;
static constexpr unsigned long long THREE_QUARTER_SECOND = 45;
static constexpr unsigned long long ONE_SECOND = 60;

static constexpr unsigned long long FALL_RATE_INITIAL = ONE_SECOND;
static constexpr unsigned long long LOCK_DELAY = HALF_SECOND;
static constexpr unsigned long long BLOCK_COLLISION_DELAY = QUARTER_SECOND;
static constexpr unsigned long long LINE_CLEAR_DELAY = THREE_QUARTER_SECOND;
static constexpr unsigned long long HUD_SPRITE_DELAY = THREE_QUARTER_SECOND;

static constexpr unsigned long long MAX_LEVEL = 20;
static constexpr unsigned long long MAX_SCORE = 9999999;
static constexpr unsigned long long MAX_LINES = 9999;

static constexpr long long CELLS_VERTICAL = 20;
static constexpr long long CELLS_HORIZONTAL = 10;

static constexpr int NUM_ROTATE_TESTS = 5;
static constexpr int NUM_ORIENTATIONS = 4;

static constexpr unsigned long long DAS_INITIAL = 20;
static constexpr unsigned long long DAS_RATE = 1;

enum class PieceTypes {
    I_Piece = 0,
    J_Piece = 1,
    L_Piece = 2,
    O_Piece = 3,
    S_Piece = 4,
    T_Piece = 5,
    Z_Piece = 6
};

enum class T_SpinTypes {
    NoSpin,
    MiniSpin,
    FullSpin
};
#endif