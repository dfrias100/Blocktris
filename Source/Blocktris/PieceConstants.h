#ifndef PIECECONSTANTS_H
#define PIECECONSTANTS_H

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

static constexpr int FontWidth = 25;
static constexpr int FontHeight = 31;


enum class PieceTypes {
    I_Piece = 0,
    J_Piece = 1,
    L_Piece = 2,
    O_Piece = 3,
    S_Piece = 4,
    T_Piece = 5,
    Z_Piece = 6
};

#endif