
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

#include "XmlAccessor.hpp"

namespace spc {
	// メインプロシージャ
	void MenuConcierge::onInitialize(){
		
		
		// オイシイ、ウマイ、ウッマのいずれかを認識する
 		SPC_ANSWER answer;
 		std::vector<std::string> answerWords;
 		std::string recogWord;
 		int recogIndex;

 		// 認識したい言葉を全角カタカナで追加する
 		answerWords.push_back("オイシイ");
 		answerWords.push_back("ウマイ");
 		answerWords.push_back("ウッマ");

 		// 質問をする
 		long rtn;
 		rtn = waitForAnswer("きょーうのごはんはなーにかなー？",
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
 	
     		// 認識した単語は、recogWord、および answerWords[recogIndex] で取得できる
     		break;

   		case SPC_ANSWER_CANCEL:
     		// ここに「やめる」を認識した時の処理を記述する
 	
 	 		speak("やめる");
 	
     		break;

   		case SPC_ANSWER_TIMEOUT:
     		// ここに質問処理がタイムアウトした時の処理を記述する
 	
 	 		speak("タイムアウト");
 	
     		break;

   		default:
     		break;
 		}
	}	
}
