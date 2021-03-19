#pragma once

namespace KismetFunctions
{

	inline auto ImportPngAsTexture2D(const wchar_t* FileFullPath)
	{
		if (!KismetRenderingLibrary || Util::IsBadReadPtr(KismetRenderingLibrary))
		{
			KismetRenderingLibrary = FindObject<UObject*>(XOR(L"KismetRenderingLibrary /Script/Engine.Default__KismetRenderingLibrary"));
		}

		auto fn = FindObject<UFunction*>(XOR(L"Function /Script/Engine.KismetRenderingLibrary.ImportFileAsTexture2D"));

		ObjectFinder EngineFinder = ObjectFinder::EntryPoint(uintptr_t(GEngine));
		ObjectFinder GameViewPortClientFinder = EngineFinder.Find(XOR(L"GameViewport"));
		ObjectFinder WorldFinder = GameViewPortClientFinder.Find(XOR(L"World"));

		UKismetRenderingLibrary_ImportFileAsTexture2D_Params params;
		params.WorldContextObject = WorldFinder.GetObj();
		params.Filename = FileFullPath;

		ProcessEvent(KismetRenderingLibrary, fn, &params);

		return params.ReturnValue;
	}

	inline auto GetFName(const wchar_t* String)
	{
		if (!KismetStringLibrary || Util::IsBadReadPtr(KismetStringLibrary))
		{
			KismetStringLibrary = FindObject<UObject*>(XOR(L"KismetStringLibrary /Script/Engine.Default__KismetStringLibrary"));
		}

		auto fn = FindObject<UFunction*>(XOR(L"Function /Script/Engine.KismetStringLibrary.Conv_StringToName"));

		UKismetStringLibrary_Conv_StringToName_Params params;
		params.inString = String;

		ProcessEvent(KismetStringLibrary, fn, &params);

		return params.ReturnValue;
	}
}