#pragma once
// Created with ReClass.NET 1.2 by KN4CK3R
// Padding technique by Public Void + Can1357 and other sources from the GuidedHacking forums
// modified for excellence
#define STR_MERGE_IMPL(a, b) a##b
#define STR_MERGE(a, b) STR_MERGE_IMPL(a, b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]
#define DEFINE_MEMBER_N(type, name, offset) struct {unsigned char MAKE_PAD(offset); type name;}

#include<cstdint>
#include<cstdio>
#include<windows.h>
#include "geom.h"

struct Vector2 { float x, y; };



class Ent
{
public:

	union
	{
		DWORD vTable;
		DEFINE_MEMBER_N(Vector3L, HeadPos, 0x4);
		DEFINE_MEMBER_N(float, speedx, 0x10);
		DEFINE_MEMBER_N(float, speedy, 0x14);
		DEFINE_MEMBER_N(float, speedz, 0x18);
		DEFINE_MEMBER_N(Vector3L, BodyPos, 0x28);
		DEFINE_MEMBER_N(Vector2, yawpitch, 0x34);
		DEFINE_MEMBER_N(float, RecoilKnockback, 0x40);
		DEFINE_MEMBER_N(int8_t, jumpBool, 0x5D);
		DEFINE_MEMBER_N(int8_t, standingBool, 0x65);
		DEFINE_MEMBER_N(int8_t, ForwardBackward, 0x74);
		DEFINE_MEMBER_N(int8_t, RightLeft, 0x75);
		DEFINE_MEMBER_N(bool, left, 0x80);
		DEFINE_MEMBER_N(bool, right, 0x81);
		DEFINE_MEMBER_N(bool, forward, 0x82);
		DEFINE_MEMBER_N(bool, backward, 0x83);
		DEFINE_MEMBER_N(int32_t, Health, 0xEC);
		DEFINE_MEMBER_N(int32_t, Armor, 0xF0);
		DEFINE_MEMBER_N(bool, shoot, 0x204);
		DEFINE_MEMBER_N(char, Username[11], 0x205);
		DEFINE_MEMBER_N(int8_t, TeamIdentify, 0x30C);
		DEFINE_MEMBER_N(class Gun*, GunEquipped, 0x368); //reorder class initalization if class gun doesnt make sense
	};


};





class Gun
{
public:
	union
	{
		DEFINE_MEMBER_N(class WeaponStats*, GunType, 0xC);
		//DEFINE_MEMBER_N(class Ammo*, AmmoPtr, 0x10); doesnt work because the offset orders are different for each gun, you could make if statements for every gun id but lazy lol
	};
};


class WeaponStats
{
public:
	union
	{
		DEFINE_MEMBER_N(int8_t, horizontalSpread, 0x5C);
		DEFINE_MEMBER_N(int8_t, verticleSpread, 0x5E);
	};

};

