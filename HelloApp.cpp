#include "TransientApplication.h"
#include "LogCtrl.h"

class CHelloApp : public PAPI::CTransientApplication
{
public:
	void OnProcessMain(Sapie::CControllerBase* pSender)
	{
		// 発話＆シナリオ実行のパラメータ準備
		// コミュニケーションパラメータクラスに、発話内容「ハローワールド」と、
		// 実行するモーションのactファイルのフルパスをセットする
		PAPI::CCommParams cParam;
		cParam.Set(PAPI::PARAM_SENTENCE, std::string("ハローワールド"));
		cParam.Set(PAPI::PARAM_MOTION_FILE, std::string("/usr/palro/etc/apl/HelloApp/HelloApp.act"));

		// 発話＆モーション実行開始
		long lReqno = cComm.Send(PAPI::COMTYP_TELL, NULL, cParam);

		// 発話＆モーションの完了待ち
		long lResult = evDispatcher.Sync(lReqno);

		// エラー判定
		// 発話とモーションが正常に完了しなかった場合に
		// ログファイルにエラー情報を出力する
		if (lResult != 0) {
			SAPIE_LOG_INF("Error: %ld", lResult);
		}
	}
};

void AppMain(Sapie::CApplicationInitializer& initializer)
{
	// アプリケーションのインスタンスを作成して、APIへ通知する
	initializer.SetApplication(new CHelloApp());
}
