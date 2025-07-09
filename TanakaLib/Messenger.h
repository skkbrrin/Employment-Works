#pragma once
#ifndef MESSENGER_DEFINED
#define MESSENGER_DEFINED
#include "Message.h"
#include "IObject.h"
#include <unordered_map>

// Messengerクラス
class Messenger
{
public:
	// オブジェクトを取得する
	IObject* GetObject(int objectID);

public:
	// Messengerクラスのインスタンスを取得する
	static Messenger* GetInstance();
	// Messengerクラスのインスタンスを破棄する
	static void DestroyInstance();
	// オブジェクトを登録する
	void Register(int objectID, IObject* object);
	// オブジェクトの登録を解除する
	void UnRegister(int objectID);
	// メッセージを送信する
	void Notify(int objectID, Message::MessageID);

private:
	// コピー・ムーブ禁止
	Messenger(const Messenger&) = delete;
	Messenger& operator=(const Messenger&) = delete;
	Messenger(Messenger&&) = delete;
	Messenger& operator=(Messenger&&) = delete;
	// コンストラクタ
	Messenger();

private:
	// メッセンジャーインスタンス
	static std::unique_ptr<Messenger> s_messenger;
	// オブジェクトIDとオブジェクトマッピング
	std::unordered_map<int, IObject*> m_objects;
};

#endif		// MESSENGER_DEFINED

