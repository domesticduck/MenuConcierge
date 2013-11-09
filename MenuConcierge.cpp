/**
 * @file MenuConcierge.cpp
 * @brief SPCフレームワークサンプルアプリケーション、メインファイル
 *
 * @note 本サンプルプログラムの最終行にある変数宣言は非常に重要です。
 * SPCフレームワークは、「spcApp」と言う固定の変数に対して処理を行うように作られています。
 */

#include "spc/spcbase.h"

/**
 * 顔検出イベント、動体検知イベントに反応して、PALROと簡単な会話を行うサンプルです。
 *
 * 顔検出イベントで通知された名前を記憶し、動体検知イベントの時に呼びかけます。
 * また、呼びかけは、 『 はい。』、『 いいえ。 』、『 やめる。 』 の3択形式の質問要求APIを使用して行い、
 * 『 やめる。 』と答えた時にアプリケーションを終了します。
 * 顔検出後に、顔認識ができなくなった場合にも、顔検出イベントで通知された名前で呼びかけます。
 *
 */
class MenuConcierge : spc::SPCBase {

  private:
    std::string face_catch_name;

  public:

    /**
     * @brief SPC起動時にフレームワークにより呼び出されるインタフェース。
     *
     */
    void onInitialize() {
        long    result;

        face_catch_name = "";

        // 発話は必須ではありませんが、アプリケーション起動確認のために発話させています。
        result = speak("サンプル起動しました");

        // 動体検知を開始
        result = startDetectMovingObject();
        if(result != 0){
            // 発話API呼び出し
            result = speak("動体検知開始は失敗しました、"
            "動体検知ができないためアプリケーションを終了します。");

            // アプリケーション終了API呼び出し
            result = exitComponent();
        }
    }

    /**
     * @brief 人の顔を検出した際にフレームワークにより呼び出されるインタフェース
     * @param name 認識した個人の登録名(全角カタカナ). 個人未確定の場合は空白文字列
     * @param owner 認識した個人がPALROのオーナーであるかどうか. オーナーの場合はtrue
     *
     */
    void onFaceCatch(std::string name, bool owner) {
        long    result;
        std::string str;

        /*
         * 顔識別結果の登録名が格納されている場合は、登録名で呼びかけます。
         */
        if(name != ""){       // 登録者の場合
            // 発話内容組み立て
            face_catch_name = name + "さん";
            str = name + "、こんにちわ";
        }
        else{   // 未知の人の場合
            face_catch_name = "あのー";
            str = "どなたかわかりませんが、こんにちわ";
        }

        // 発話API呼び出し
        result = speak(str);
    }

    /**
     * @brief onCatchedFace後に、顔を見失った際にフレームワークにより呼び出されるインタフェース
     *
     * 顔検出が行われた後であれば、顔検出で設定した呼び名を2回発話します。
     */
    void onFaceDrop() {
        if(face_catch_name != ""){
            long    result;

            // 発話API呼び出し
            result = speak(face_catch_name);
            result = speak("どこですかー？");
        }
    }

    /**
     * @brief 動体を検知した際にフレームワークにより呼び出されるインタフェース
     * @param speed 動体移動速度
     * @param direction 検知した動体の方向
     *
     * 動体を検知したら、顔検出で設定した呼び名を使用して、動体検知を知らせます。
     */
    void onMovingObjectCatch(float speed, spc::SPC_MOVING_OBJECT_DIRECTION direction)
    {
        long    result;

        if(face_catch_name != ""){
            spc::SPC_ANSWER ans_result;

            // 発話API呼び出し
            result = speak(face_catch_name);

            /*
             * 質問要求(Yes/No)
             *  『 はい。』、『 いいえ。 』、『 やめる。 』 で回答する質問をする
             */
            result = waitForAnswer("今、何か動きましたか？", ans_result);

            // 質問結果確認
            if(result == spc::SPC_ANSWER_RECOGEND){
                switch(ans_result){
                case spc::SPC_ANSWER_YES:   // 『 はい。』 と回答された
                    result = speak("やっぱりそうですか。");
                    break;
                case spc::SPC_ANSWER_NO:        // 『 いいえ。』 と回答された
                    result = speak("おかしいなぁ。");
                    break;
                case spc::SPC_ANSWER_CANCEL:    // 『 やめる。』 と回答された
                    result = speak("わかりました、アプリケーションを終了します。");

                    // アプリケーション終了API呼び出し
                    result = exitComponent();
                    break;
                default:
                    break;
                }
            }
        }
    }

    /**
     * @brief SPC終了時にフレームワークにより呼び出されるインタフェース。
     *
     * onInitializeで開始した動体検知を終了させ、SPCアプリケーションを終了させる。
     */
    void onFinalize() {
        long    result;

        // onInitializeで動体検知を開始したので、動体検知を終了する
        result = stopDetectMovingObject();

        // SPC終了
        //exit(0);
    }

};

// SPCアプリケーションインスタンス生成
MenuConcierge   spcApp;