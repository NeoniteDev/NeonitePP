#pragma once

#define PI (3.141592653589793f)

//Memory
namespace Patterns
{
	constexpr const char* CurlEasySetOpt = "\x89\x54\x24\x10\x4C\x89\x44\x24\x18\x4C\x89\x4C\x24\x20\x48\x83\xEC\x28\x48\x85\xC9\x75\x08\x8D\x41\x2B\x48\x83\xC4\x28\xC3\x4C";
	constexpr const char* CurlSetOpt = "\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x48\x83\xEC\x30\x33\xED\x49\x8B\xF0\x48\x8B\xD9";
	constexpr const char* GEngine = "\x48\x89\x74\x24\x20\xE8\x00\x00\x00\x00\x48\x8B\x4C\x24\x40\x48\x89\x05";
	constexpr const char* GObjects = "\x48\x8B\x05\x7E\x38\x79\x05\x4C\x8D\x34\xCD";
	constexpr const char* UWorld = "\x48\x8B\x1D\x00\x00\x00\x00\x48\x85\xDB\x74\x3B\x41";
	constexpr const char* SCOI =
	"\x4C\x89\x44\x24\x18\x55\x53\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4";
	constexpr const char* ProcessEvent = "\x40\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x81\xEC\x00\x00\x00\x00\x48\x8D\x6C\x24\x00\x48\x89\x9D\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC5\x48\x89\x85\x00\x00\x00\x00\x8B\x41\x0C\x45\x33\xF6\x3B\x05\x00\x00\x00\x00\x4D\x8B\xF8\x48\x8B\xF2\x4C\x8B\xE1\x41\xB8\x00\x00\x00\x00\x7D\x2A";
	constexpr const char* GONI = "\x40\x53\x48\x83\xEC\x20\x48\x8B\xD9\x48\x85\xD2\x75\x45\x33\xC0\x48\x89\x01\x48\x89\x41\x08\x8D\x50\x05\xE8\x00\x00\x00\x00\x8B\x53\x08\x8D\x42\x05\x89\x43\x08\x3B\x43\x0C\x7E\x08\x48\x8B\xCB\xE8\x00\x00\x00\x00\x48\x8B\x0B\x48\x8D\x15\x00\x00\x00\x00\x41\xB8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x8B\xC3\x48\x83\xC4\x20\x5B\xC3\x48\x8B\x42\x18";
	constexpr const char* GetViewPoint = "\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x20\x48\x8B\xD9\x41\x8B\xF0\x48\x8B\x49\x30\x48\x8B\xFA\xE8\x00\x00\x00\x00\xBA\x00\x00\x00\x00\x48\x8B\xC8";
};

namespace Masks
{
	constexpr const char* CurlEasySetOpt = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
	constexpr const char* CurlSetOpt = "xxxxxxxxxxxxxxxxxxxxxxxxxxxx";
	constexpr const char* GEngine = "xxxxxx????xxxxxxxx";
	constexpr const char* GObjects = "xxx????xxxx";
	constexpr const char* UWorld = "xxx????xxxxxx";
	constexpr const char* SCOI = "xxxxxxxxxxxxxxxxxxxxx????xxx????xxx????xxx";
	constexpr const char* ProcessEvent = "xxxxxxxxxxxxxxx????xxxx?xxx????xxx????xxxxxx????xxxxxxxx????xxxxxxxxxxx????xx";
	constexpr const char* GONI = "xxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxx????xxxxxx????xx????x????xxxxxxxxxxxxx";
	constexpr const char* GetViewPoint = "xxxx?xxxx?xxxxxxxxxxxxxxxxxxx????x????xxx";
};

namespace Offsets
{
	constexpr DWORD GameInstance = 0x180;
	constexpr DWORD LocalPlayers = 0x38;
	constexpr DWORD PlayerController = 0x30;
}

//UE4
enum EObjectFlags
{
	RF_NoFlags = 0x00000000,
	RF_Public = 0x00000001,
	RF_Standalone = 0x00000002,
	RF_MarkAsNative = 0x00000004,
	RF_Transactional = 0x00000008,
	RF_ClassDefaultObject = 0x00000010,
	RF_ArchetypeObject = 0x00000020,
	RF_Transient = 0x00000040,
	RF_MarkAsRootSet = 0x00000080,
	RF_TagGarbageTemp = 0x00000100,
	RF_NeedInitialization = 0x00000200,
	RF_NeedLoad = 0x00000400,
	RF_KeepForCooker = 0x00000800,
	RF_NeedPostLoad = 0x00001000,
	RF_NeedPostLoadSubobjects = 0x00002000,
	RF_NewerVersionExists = 0x00004000,
	RF_BeginDestroyed = 0x00008000,
	RF_FinishDestroyed = 0x00010000,
	RF_BeingRegenerated = 0x00020000,
	RF_DefaultSubObject = 0x00040000,
	RF_WasLoaded = 0x00080000,
	RF_TextExportTransient = 0x00100000,
	RF_LoadCompleted = 0x00200000,
	RF_InheritableComponentTemplate = 0x00400000,
	RF_DuplicateTransient = 0x00800000,
	RF_StrongRefOnFrame = 0x01000000,
	RF_NonPIEDuplicateTransient = 0x02000000,
	RF_Dynamic = 0x04000000,
	RF_WillBeLoaded = 0x08000000,
};

enum EInternalObjectFlags
{
	None = 0,
	ReachableInCluster = 1 << 23,
	ClusterRoot = 1 << 24,
	Native = 1 << 25,
	Async = 1 << 26,
	AsyncLoading = 1 << 27,
	Unreachable = 1 << 28,
	PendingKill = 1 << 29,
	RootSet = 1 << 30,
	GarbageCollectionKeepFlags = Native | Async | AsyncLoading,
	AllFlags = ReachableInCluster | ClusterRoot | Native | Async | AsyncLoading | Unreachable | PendingKill | RootSet,
};


enum class ECameraProjectionMode : uint8_t
{
	Perspective = 0,
	Orthographic = 1,
	ECameraProjectionMode_MAX = 2
};