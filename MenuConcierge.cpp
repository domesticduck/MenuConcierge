
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

using namespace Model;

namespace spc {
	// メインプロシージャ
	void MenuConcierge::onInitialize(){
		
		setLogLevel(SPC_LOG_LEVEL_TRACE); 
    SPC_LOG_DEBUG("*** start ***");
    Menus menus; 
    Menus sub_menus; 
    Menus sub_menus2; 

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
    rtn = waitForAnswer("今日もおいしそうなご飯ですねー",
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
      rtn = waitForAnswer("今日の献立は何ですか？", free_answer, recog);
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

        //メニューをrailsへ保存する。
        menus.name = recog;
        menus.create();         

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

 					//サブメニューとメインメニューをひもづけて保存する。
          sub_menus.name = recog_sub;
          sub_menus.main_id = menus.id;
          sub_menus.create();

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
 						if(recog_sub != "ないよ"){
              sub_menus2.name = recog_sub;
              sub_menus2.main_id = menus.id;
              sub_menus2.create();
 							speak("ふむふむ");
 						}
 					}	
 					while(recog_sub != "ないよ");
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
