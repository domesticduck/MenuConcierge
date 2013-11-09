
#pragma once

#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <string>
#include <vector>

#include <spc/spcbase.h>

#define LOG SPC_LOG_ERROR

namespace Util
{
	class CXmlAccessor {
	public:
		CXmlAccessor(){
			m_pDoc = NULL;
		}
		virtual ~CXmlAccessor(){
			ClearnUp();
		}
		long Load(std::string sFileName){
			if(sFileName.empty()){
				return -1;
			}
			// クリア.
			ClearnUp();
			// XML読み込み.
			m_pDoc = xmlParseFile(sFileName.c_str());
			if(m_pDoc == 0){
				LOG(" ERR: XMLパースエラー. file=%s.", sFileName.c_str());
				return 1;
			}
			return 0;
		}
		long GetValues(std::string sXPath, std::vector<std::string>& sValueList){
			if(m_pDoc == NULL){
				LOG(" ERR: XML未読み込み.");
				return -1;
			}
			long lRes = 1;
			xmlXPathContextPtr pContext = xmlXPathNewContext(m_pDoc);
			if(pContext != NULL){
				xmlXPathObjectPtr pObject = xmlXPathEvalExpression((xmlChar*)sXPath.c_str(), pContext);
				if(pObject != NULL){
					xmlNodeSetPtr pNodeSet= pObject->nodesetval;
					if(pNodeSet != NULL){
						const int numNode = pNodeSet->nodeNr;
						for(int nn=0; nn<numNode; nn++){
							xmlNode* node = pNodeSet->nodeTab[nn];
							if(node != NULL){
								char* psz = (char*)xmlNodeGetContent(node);
								if(psz != NULL){
									std::string value = std::string(psz);
									//LOG(" [%d] XML読込OK. xpath(%s) >> val=%s.", nn, sXPath.c_str(), value.c_str());
									sValueList.push_back( value );
									lRes = 0;
									xmlFree(psz);
								}else{
									LOG(" ERR: xmlNodeGetContent() returns NULL. xpath=%s.", sXPath.c_str());
								}
							}else{
								LOG(" ERR: pNodeSet->nodeTab[%d] is NULL. xpath=%s.", nn, sXPath.c_str());
							}
						} // for(nn).
						if(numNode == 0){
							LOG(" ERR: pNodeSet->nodeNr is 0. xpath=%s.", sXPath.c_str());
						}
					}else{
						LOG(" ERR: pObject->nodesetval is NULL. xpath=%s.", sXPath.c_str());
					}
					xmlXPathFreeObject(pObject);
				}else{
					LOG(" ERR: xmlXPathEvalExpression(%s) returns NULL.", sXPath.c_str());
				}
				xmlXPathFreeContext(pContext);
			}else{
				LOG(" ERR: xmlXPathNewContext() returns NULL. xpath=%s.", sXPath.c_str());
			}
			return lRes;
		}
		
	protected:
		void ClearnUp(){
			if(m_pDoc != NULL){
				xmlFreeDoc(m_pDoc);
				xmlCleanupParser();
			}
			m_pDoc = NULL;
		}
	protected:
		xmlDocPtr m_pDoc;
	};
	
}
