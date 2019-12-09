// Fill out your copyright notice in the Description page of Project Settings.

#include "EscapeRoom.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, EscapeRoom, "EscapeRoom" );


// Pointers primer
// Pointers are memory address; returning their value only returns the address not the actual value located there

// Syntax
// FType* PointerName;  or   FType * PointerName  or  FType *PointerName;   any one works but first one is more common

// Dereference the pointer with *PointerName to get the value stored at that address
// We could access member functions and such with (*PointerName).SomeFunct();
// ORRRRRR we can use PointerName->SomeFunct();
