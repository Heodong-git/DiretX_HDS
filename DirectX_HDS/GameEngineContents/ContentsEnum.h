#pragma once

enum class RenderOrder
{
	BACKGROUND,
	MAP,
	DOOR,
	MONSTER,
	BOSS,
	UI,
	PLAYER,
	PLAYER_EFFECT,
	EFFECT,
	DEBUG,
	CURSOR,
};

enum class ColOrder
{
	NONE,
	PLAYER,
	PLAYER_ATTACK,
	PLAYER_DOOR_BREAK,
	MONSTER,
	MONSTER_ATTACK,
	CHECKBOX,
	CURSOR,
	DOOR,
	STAGE_COL_OBJ,
	PLATFORM,
};