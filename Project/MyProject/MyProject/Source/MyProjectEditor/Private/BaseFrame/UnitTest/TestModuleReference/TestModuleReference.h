#pragma once

#include "MyProjectEditor/Private/BaseFrame/UnitTest/TestModuleReference/TestModuleReferenceSubDir/TestModuleReferenceSubDir.h"

// Module.MyPlugin.cpp.obj : error LNK2019: unresolved external symbol "public: __cdecl TestModuleReferenceSubDir::TestModuleReferenceSubDir(void)" (??0TestModuleReferenceSubDir@@QEAA@XZ) referenced in function "public: __cdecl ATestPluginCharacter::ATestPluginCharacter(void)" (??0ATestPluginCharacter@@QEAA@XZ)
// MyProjectEditor �����Ҫ��������ã� ��Ҫ���� MYPROJECTEDITOR_API ������ᵼ�±��벻��
class MYPROJECTEDITOR_API TestModuleReference : TestModuleReferenceSubDir
{
public:
	TestModuleReference();
};