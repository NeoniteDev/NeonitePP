#pragma once
#include "cryptolens/core.hpp"
#include "cryptolens/Error.hpp"
#include "cryptolens/Configuration_Windows.hpp"
#include "cryptolens/MachineCodeComputer_static.hpp"

#define TOTAL_BYTES_READ  1024
#define MAXLENGTH 100

namespace cryptolens = ::cryptolens_io::v20190401;
using Cryptolens = cryptolens::basic_Cryptolens<cryptolens::Configuration_Windows<cryptolens::MachineCodeComputer_static>>;

namespace HWID
{
	inline auto GetSystemDriveLetter()
	{
		std::string sysdrive = getenv(XOR("SystemDrive"));

		if (sysdrive.empty())
		{
			MessageBoxW(nullptr, XOR(L"Couldn't find the system drive letter."), XOR(L"Cranium HWID System"), MB_OK);
			exit(0);
		}
		else
		{
			return sysdrive;
		}
	}

	inline auto GetId()
	{
		DWORD serialNumber;
		auto systemDriveLetter = GetSystemDriveLetter() + "\\";

		if (GetVolumeInformationA(
			systemDriveLetter.c_str(),
			nullptr,
			NULL,
			&serialNumber,
			nullptr,
			nullptr,
			nullptr,
			NULL))
		{
			return serialNumber;
		}

		MessageBoxA(nullptr, XOR("Couldn't get your hwid info."), XOR("Cranium HWID System"), MB_OK);
		exit(0);
	}

	inline bool WriteKeyToReg(wchar_t* readString)
	{
		DWORD dwDisposition;
		HKEY hKey;
		const DWORD Ret =
		RegCreateKeyEx(
			HKEY_CURRENT_USER,
			XOR(L"Neonite"),
			0,
			nullptr,
			REG_OPTION_NON_VOLATILE,
			KEY_ALL_ACCESS,
			nullptr,
			&hKey,
			&dwDisposition);
		if (Ret != ERROR_SUCCESS)
		{
			printf(XOR("Error opening or creating key.\n"));
			return false;
		}
		if (ERROR_SUCCESS !=
			RegSetValueEx(
				hKey,
				XOR(L"key"),
				0,
				REG_SZ,
				reinterpret_cast<LPBYTE>(readString),
				(((static_cast<DWORD>(lstrlen(readString)) + 1)) * 2)))
		{
			RegCloseKey(hKey);
			return false;
		}
		RegCloseKey(hKey);
		return true;
	}


	inline auto ReadKeyFromReg()
	{
		HKEY hKey;
		//Check if the registry exists
		DWORD Ret = RegOpenKeyEx(
			HKEY_CURRENT_USER,
			XOR(L"Neonite"),
			0,
			KEY_READ,
			&hKey
		);
		if (Ret == ERROR_SUCCESS)
		{
			std::wstring data;

			wchar_t buffer[100];
			DWORD size = sizeof(buffer);
			DWORD type = REG_SZ;

			auto nError = RegQueryValueEx(hKey, XOR(L"key"), nullptr, &type, reinterpret_cast<LPBYTE>(buffer), &size);

			if (nError != ERROR_FILE_NOT_FOUND)
			{
				data = buffer;
				return data;
			}
		}
	}

	inline bool Validate()
	{
		cryptolens::Error e;
		Cryptolens cryptolens_handle(e);

		cryptolens_handle.signature_verifier.set_modulus_base64(
			e,
			XOR(
				"zm+58eRygXW8bBm4y6fxGJnvv4GhT7VQgimSEZQPiaASvx/mAeTHpAzO4KTyixFUopapcOYI9NW/b5cAo1VwmIM/KUlSLs2l5npGOiuN78IhMnxIy83N6xnr+iuOqlG0l+KhYavRNPkBuBMmlw5d8hm3TItr8Oas43WHq5m/gozR0EVsw8/F0tpmZMSP6KytJ3XaWbGVE9lCgcJtNQqpvzOWq/1FC47ILbSbqnjkDf7Nv3GCu+nxnxg0ZyzTrHLyo07Ntei6b1zoz8uUd5cK5tJy3engZHbFjdScjXbZAVmBlB9sOjzjf97OYQOKrx46SoLLPrb6hM+rwOJnYo6koQ=="));
		cryptolens_handle.signature_verifier.set_exponent_base64(e, XOR("AQAB"));
		std::ostringstream hwid;
		hwid << GetId();
		cryptolens_handle.machine_code_computer.set_machine_code(e, hwid.str());

		auto keyW = ReadKeyFromReg();
		auto key = std::string(keyW.begin(), keyW.end());

		if (!key.empty())
		{
			auto license_key =
			cryptolens_handle.activate(
				e,
				XOR("WyI0MDgyNTciLCJvR0cwVTZhSTg1NGxURmI5SVVBNUdFUisyRmlqVHM4U29EVlBINDR3Il0="),
				9575,
				// License Key
				key
			);

			if (license_key->check().has_feature(1))
			{
				return true;
			}
		}

		return false;
	}
}
