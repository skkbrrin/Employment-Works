#include "pch.h"
#include "GameObject/MyBinalyFile.h"

#include <fstream>
#include <assert.h>

// ファイルの読み込み
std::unique_ptr<MyBinaryFile> MyBinaryFile::LoadFile(const wchar_t * fileName)
{
	std::ifstream ifs;

	//	一番始めにファイルオープン
	ifs.open(fileName, std::ios::in | std::ios::binary);

	//	読み込み失敗時、強制終了
	assert(ifs);

	//	実際にファイル内容を読み取る
	MyBinaryFile* Mbin = new MyBinaryFile();

	//	ファイルサイズを取得
	ifs.seekg(0, std::fstream::end);
	std::streamoff eofPos = ifs.tellg();
	ifs.clear();
	ifs.seekg(0, std::fstream::beg);
	std::streamoff begPos = ifs.tellg();
	Mbin->m_size = (unsigned int)(eofPos - begPos);

	//	読み込むためのメモリを確保
	Mbin->m_data.reset(new char[Mbin->m_size]);

	//	ファイル先頭からバッファへコピー 
	ifs.read(Mbin->m_data.get(), Mbin->m_size);

	//	最後にはファイルクローズ
	ifs.close();

	return std::make_unique<MyBinaryFile>(Mbin);
}

// コンストラクタ
MyBinaryFile::MyBinaryFile()
{
	// サイズ初期値
	m_size = 0;
}

// ムーブコンストラクタ
MyBinaryFile::MyBinaryFile(MyBinaryFile* in)
{
	// データをファイル内に入れる
	m_data = std::move(in->m_data);
	m_size = in->m_size;
}
