#pragma once

enum class RenderOrder
{
	BACKGROUND,
	MAP,
	WALL,
	DOOR,
	BLOOD_EFFECT,
	MONSTER_BLOOD,
	TURRET_BODY,
	MONSTER,
	UI,
	FANBLADE,
	LASER,
	PLATFORM,
	BULLET,
	PLAYER_TRAIL,
	BOSS,
	PLAYER,
	PLAYER_EFFECT,
	BOSS_TP_EFFECT,
	EFFECT,
	BOSS_EFFECT,
	DEBUG,
	LEVELCHANGE_OBJ,
	GAME_END,
	CURSOR,
};

enum class ColOrder
{
	NONE,
	PLAYER,
	PLAYER_ATTACK,
	PLAYER_BULLET,
	PLAYER_DOOR_BREAK,
	PLAYER_SUB,
	MONSTER,
	MONSTER_ATTACK,
	MONSTER_BULLET,
	MONSTER_CHASE,
	MONSTER_CHECK,
	MONSTER_RANGE_CHECK,
	BOSS,
	BOSS_ATTACK,
	BOSS_EXPLOSION,
	CHECKBOX,
	CURSOR,
	DOOR,
	DOOR_OPEN_EVENT,
	STAGE_COL_OBJ,
	PLATFORM,
	LASER,
	FANBLADE,
	PARTITION,
	BULLET,
	SLIDINGDOOR,
	TURRET,
};