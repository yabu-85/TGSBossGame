#include <xaudio2.h>
#include <vector>
#include "Audio.h"

//なんかリトルエンディアンとかの違いがあるらしい 下見て
//https://learn.microsoft.com/ja-jp/windows/win32/xaudio2/how-to--load-audio-data-files-in-xaudio2
#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'

#define SAFE_DELETE_ARRAY(p) if(p){delete[] p; p = nullptr;}

namespace Audio
{
	//XAudio本体
	IXAudio2* pXAudio = nullptr;

	//マスターボイス
	IXAudio2MasteringVoice* pMasteringVoice = nullptr;

	//ファイル毎に必要な情報
	struct AudioData
	{
		//ソースボイス
		IXAudio2SourceVoice** pSourceVoice = nullptr;

		//同時再生最大数
		int svNum = 1;

		//ファイル名
		std::string fileName = "";
	};
	std::vector<AudioData>	audioDatas;
}

//初期化
void Audio::Initialize()
{
	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	XAudio2Create(&pXAudio, 0, XAUDIO2_DEFAULT_PROCESSOR);
	pXAudio->CreateMasteringVoice(&pMasteringVoice);

}

//サウンドファイル(.wav）をロード
int Audio::Load(std::string fileName, int svNum)
{
	//すでに同じファイルを使ってないかチェック
	for (int i = 0; i < audioDatas.size(); i++)
	{
		if (audioDatas[i].fileName == fileName)
		{
			return i;
		}
	}

	return 0;

}

//再生
void Audio::Play(std::string _fileName)
{
	//サウンド情報
	WAVEFORMATEXTENSIBLE wfx = { 0 };
	XAUDIO2_BUFFER buffer = { 0 };

	HANDLE hFile = CreateFile(_fileName.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	SetFilePointer(hFile, 0, NULL, FILE_BEGIN);

	DWORD dwChunkSize;
	DWORD dwChunkPosition;
	//check the file type, should be fourccWAVE or 'XWMA'
	FindChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);
	DWORD filetype;
	ReadChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);

	FindChunk(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
	ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);

	//fill out the audio data buffer with the contents of the fourccDATA chunk
	FindChunk(hFile, fourccDATA, dwChunkSize, dwChunkPosition);
	BYTE* pDataBuffer = new BYTE[dwChunkSize];
	ReadChunkData(hFile, pDataBuffer, dwChunkSize, dwChunkPosition);

	buffer.AudioBytes = dwChunkSize;  //size of the audio buffer in bytes
	buffer.pAudioData = pDataBuffer;  //buffer containing audio data
	buffer.Flags = XAUDIO2_END_OF_STREAM; // tell the source voice not to expect any data after this buffer

	IXAudio2SourceVoice* pSourceVoice;
	pXAudio->CreateSourceVoice(&pSourceVoice, (WAVEFORMATEX*)&wfx);

	pSourceVoice->SubmitSourceBuffer(&buffer);

	//音量の設定
	pSourceVoice->SetVolume(0.1f);

	pSourceVoice->Start(0);

}

//すべて開放
void Audio::Release()
{
	for (int i = 0; i < audioDatas.size(); i++)
	{
		for (int j = 0; j < audioDatas[i].svNum; j++)
		{
			audioDatas[i].pSourceVoice[j]->DestroyVoice();
		}
	}

	CoUninitialize();
	if (pMasteringVoice)
	{
		pMasteringVoice->DestroyVoice();
	}
	pXAudio->Release();
}

HRESULT Audio::FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());

	DWORD dwChunkType = 0;
	DWORD dwChunkDataSize = 0;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType = 0;
	DWORD bytesRead = 0;
	DWORD dwOffset = 0;

	while (hr == S_OK)
	{
		DWORD dwRead;
		if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());

		if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());

		switch (dwChunkType)
		{
		case fourccRIFF:
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL))
				hr = HRESULT_FROM_WIN32(GetLastError());
			break;

		default:
			if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT))
				return HRESULT_FROM_WIN32(GetLastError());
		}

		dwOffset += sizeof(DWORD) * 2;

		if (dwChunkType == fourcc)
		{
			dwChunkSize = dwChunkDataSize;
			dwChunkDataPosition = dwOffset;
			return S_OK;
		}

		dwOffset += dwChunkDataSize;

		if (bytesRead >= dwRIFFDataSize) return S_FALSE;

	}

	return S_OK;
}

HRESULT Audio::ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());
	DWORD dwRead;
	if (0 == ReadFile(hFile, buffer, buffersize, &dwRead, NULL))
		hr = HRESULT_FROM_WIN32(GetLastError());
	return hr;
}
