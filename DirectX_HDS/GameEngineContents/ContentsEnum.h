#pragma once

enum class RenderOrder
{
	BACKGROUND,
	MAP,
	UI,
	MONSTER,
	BOSS,
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
	MONSTER,
	MONSTER_ATTACK,
	CURSOR,
};