#pragma once
#ifndef MESSAGE_DEFINED
#define MESSAGE_DEFINED

// Messageクラス
// ・IDLINGをOFFENSIVE_IDLINGに変更する
class Message
{
public:
	// メッセージID
	enum MessageID : int  
	{ 
		// サッカーボール
		TOUCH = 0,									// 「ボールに触れる」活動
		KICK_OFF,									// 「キックオフする」活動
		KICK,										// 「キックする」活動
		STOPPING,									// 「停止」状態
		MOVING,										// 「移動」状態

		// プレイヤー
		OFFENSIVE_IDLING = 10,				        // 「オフェンシブアイドリング」状態
		IDLING,								        // 「アイドリング」状態
		KICKING_OFF,						        // 「キッキングオフ」状態
		KICKING_BALL,						        // 「キッキングボール」状態
		CHACHING_BALL,                              // 「キャッチングボール」状態
	};
};

#endif		// MESSAGE_DEFINED

