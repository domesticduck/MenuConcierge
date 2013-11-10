
#include "MenuConcierge.h"
#include <spc/spcbase.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/URI.h>

#include "Menus.hpp"

namespace spc {
	// メインプロシージャ
	void MenuConcierge::onInitialize(){
		
		setLogLevel(SPC_LOG_LEVEL_TRACE); 
    SPC_LOG_DEBUG("*** start ***");

    std::string ans;

    Model::Menus menus("http://192.168.3.7:3000/menus.text");
    menus.get(ans); 

    speak(ans);

		// オイシイ、ウマイ、ウッマのいずれかを認識する
 		SPC_ANSWER answer;
		SPC_ANSWER free_answer;	
 		std::vector<std::string> answerWords;
 		std::string recogWord;
		std::string recog;
		std::string recog_sub;		
 		int recogIndex;

 		// 認識したい言葉を全角カタカナで追加する
		// オイシイ、ウマイ、ウッマのいずれかを認識する
		answerWords.push_back("オイシイ");
 		answerWords.push_back("ウマイ");
 		answerWords.push_back("ウッマ");

 		// 質問をする
 		long rtn;
 		rtn = waitForAnswer("きょうのごはんはなにかなー？",
              answerWords,
              answer,
              recogWord,
              recogIndex);
 		if(rtn != 0){
   		// waitForAnswer処理失敗
   		// アプリケーションの終了
   		exitComponent();
   		return;
 		}
 		switch(answer){
   		case SPC_ANSWER_RECOGEND:
     		// ここに質問が正常終了した場合の処理を記述する
 	
 			speak("おいしそうですねー。");
 			
 			 // 質問をする
 			long rtn;
 			rtn = waitForAnswer("きょうのこんだては何ですか？", free_answer, recog);
 			if(rtn != 0){
   			// waitForAnswer処理失敗
   			// アプリケーションの終了
   			exitComponent();
   			return;
 			}
 			switch(free_answer){
   			case SPC_ANSWER_FREE_WORD:
     			// 自由回答を認識した
 	
     			// 認識結果は recog に格納される。
 	 			speak(recog + "ですか");
 				speak("なるほど！なるほど！");

 				//TODO: recogの内容railsへ保存する。
 				//TODO: String型の文字列(ここでは変数のrecogを渡して、railsで保存するメソッド)返り値で保存の成否と保存したメインメニューのID？
 	
 				// 質問をする
 				long rtn;
 				rtn = waitForAnswer("その他のおかずは何ですか？", free_answer, recog_sub);
 				if(rtn != 0){
   				// waitForAnswer処理失敗
   				// アプリケーションの終了
   				exitComponent();
   				return;
 				}
 				
 				switch(free_answer){
   				case SPC_ANSWER_FREE_WORD:
     				// 自由回答を認識した
 	
     				// 認識結果は recog_sub に格納される。
 	 				speak(recog_sub + "ですね");
 					speak("うん！うん！");

 					//TODO: recog_subの内容とメインメニューのIDをrailsへ渡し紐付けて保存する。
 					//TODO: String型の文字列とメインメニューのIDを渡す、返り値を受けて保存に成功：失敗

 					do{
 						// 質問をする
 						long rtn;
 						rtn = waitForAnswer("他にはありますか？", free_answer, recog_sub);
 						if(rtn != 0){
   							// waitForAnswer処理失敗
   							// アプリケーションの終了
   							exitComponent();
   							return;
						}
 						if(recog_sub != "いいえ"){
 							//TODO: recog_subの内容とメインメニューのIDをrailsへ渡し紐付けて保存する。
 							//TODO: String型の文字列とメインメニューのIDを渡す、返り値を受けて保存に成功：失敗
 							speak("ふむふむ");
 						}
 					}	
 					while(recog_sub != "いいえ");
 						speak("なるほどー");
 						break;
 					
   				case SPC_ANSWER_TIMEOUT:
     				// 質問がタイムアウトした
 	
 					speak("返答が無かったので終了しました。");
 	
     			break;

   				case SPC_ANSWER_RETRYOUT:
     				// 質問を再確認したが回答が聞き取れなかった
 	
 					speak("回答が聞き取れませんでした");
 	
     				break;

   				default:
     				break;
 				}
 			
     			break;

   			case SPC_ANSWER_TIMEOUT:
     			// 質問がタイムアウトした
 	
 				speak("返答が無かったので終了しました。");
 	
     		break;

   			case SPC_ANSWER_RETRYOUT:
     			// 質問を再確認したが回答が聞き取れなかった
 	
 				speak("回答が聞き取れませんでした");
 	
     			break;

   			default:
     			break;
 			}
 			
     		// 認識した単語は、recogWord、および answerWords[recogIndex] で取得できる
     		break;

   		case SPC_ANSWER_CANCEL:
     		// ここに「やめる」を認識した時の処理を記述する
 	
 	 		speak("やめました");
 	
     		break;

   		case SPC_ANSWER_TIMEOUT:
     		// ここに質問処理がタイムアウトした時の処理を記述する
 	
 	 		speak("返答が無かったので終了しました");
 	
     		break;

   		default:
     		break;
 		}
	}	
}
