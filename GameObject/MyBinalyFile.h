/// <summary>
/// 	BinaryFile.h
///
///	CSOをはじめとした、バイナリ形式で保存されたファイルを読み込み、管理するクラス。
///	ファイルデータをchar配列としてm_dataとして確保し続けるため、何らかの形で重複を避ける管理の仕組みが必要。
/// </summary>
#pragma once

#include <memory>

	class MyBinaryFile
	{
	protected:

		//	データ
		std::unique_ptr<char[]> m_data;

		//	サイズ
		unsigned int m_size;

	public:

		//	ファイル名を指定してロード
		static std::unique_ptr<MyBinaryFile> LoadFile(const wchar_t* fileName);

		MyBinaryFile();

		//	ムーブコンストラクタ
		MyBinaryFile(MyBinaryFile* in);

		//	アクセサ(取得)
		char* GetData() { return m_data.get(); }
		unsigned int GetSize() { return m_size; }
	};
