#include "rcmanager.h"

BYTE	checkByte11;

WORD	checkWord10;
#define CHECK_WORD10

BYTE	checkByte1_19;

WORD	checkWord10_1;
#define CHECK_WORD10_1

BYTE	checkByte1_8;

void RCManager::Initialize(HWND hwnd, bool bdbg){

	Clear();
	
	hWnd			= hwnd;
	bRCDebug		= bdbg;
		
	dwScanLogCount  = dwParseLogCount = 0;
	
	// assign new id
	dwRCId			= RCIdCount;
	RCIdCount++;

}

// default constructor
RCManager::RCManager(){
		
	dwScanLogCount = dwParseLogCount = 0;
	
	// assign new id
	dwRCId = RCIdCount;
	RCIdCount++;

	LastAccessedCategory = NULL;
	bRCDebug = false;
}

// main constructor
RCManager::RCManager(HWND hwnd, bool bdbg){
	
	Initialize(hwnd, bdbg);
	LastAccessedCategory = NULL;
}


// scan file, get tokens and save them to the table	
RC_RETURN_CODE RCManager::ScanFile(TCHAR *filename, rc_token **table, DWORD *tb_size){
	
	FILE			*fp, *fplog;
	RC_SCAN_STATE	scan_state;
	RC_SCAN_CLASS	symb_class;
	DWORD			token_count = 0;
	int				symb,token_index;
	char			buf[2048],*endptr;
	TCHAR			wbuf[2048]; 
	DWORD			buf_index,i;
	DWORD			row_count = 0;
	rc_token		*token;
	
	// open file fo scanning
	for(i=(DWORD)_tcslen(filename);i>0;i--){
		if(filename[i] == '.')break;
	}
	
	if(i == 0)
		FileName = filename;
	else{
		_stprintf_s(wbuf, 2048, TEXT("%s"),filename);
		wbuf[i] = 0;
		FileName = wbuf;
	} 
		
	_tfopen_s(&fp, filename, TEXT("rb"));
	if(fp == NULL)
		return 	RC_ERROROPENFILE;

	if(this->bRCDebug){	// if required debug information 
		
		// generate log file name
		_stprintf_s(wbuf, 2048, TEXT("%s_scan_%3.3d_%3.3d.log"),FileName.getCString(), this->dwRCId, this->dwScanLogCount);
		_tfopen_s(&fplog, wbuf,TEXT("w"));		
		if(fplog == NULL){
			fclose(fp);
			return RC_ERROROPENLOGFILE;
		}
		
		this->dwScanLogCount ++;	// increment log count	
	}
	
	scan_state = S0;	// by default scaner state is S0
	
	for(;;){ // scan until end of file
	
		symb		= fgetc(fp);			// get symbol and ...
		symb_class  = (RC_SCAN_CLASS)symb_to_class(symb);	// its class

		switch(scan_state){
		
			case S0:	// default state
				
				if(symb == ' ')continue;	// skip space
				if(symb == 0x09)continue;	// skip tab 
				if(symb == 0x0d){			// end of line
					fgetc(fp);				// skip 0x0a				
					row_count++;			// increment row count
										
					continue;
				}
				
				if(symb == EOF){	//  end of file!
					// file succesfully scanned
					
					if(this->bRCDebug){
						fprintf(fplog,"%d : end of file",row_count);
						fclose(fplog);
					}
					
					fclose(fp);
					*tb_size = token_count;
					
					return RC_OK;
				
				}// if EOF
				
				if(symb_class == CLASS_LETTER){ //	we'v got letter from file
					buf[0] = (char)symb;
					buf_index = 1;
					scan_state = S_ID_RWORD;	// change automat state
					continue;
				}
				
				if(symb_class == CLASS_DIGIT){ // may be float or dword
					buf[0] = (char)symb;
					buf_index = 1;
					scan_state = S_NUMVALUE;	// change automat state
					continue;
				}
				
				if(symb_class == CLASS_PARSER){
					if(symb == '/'){ // may be only comment
						// get next symbol must be / or *
						symb = fgetc(fp);
						if(symb == '/'){
							scan_state	 = S_COMMENT_LINE;	// chatge state to comment	
						}
						else if(symb == '*'){
							scan_state	 = S_COMMENT_BLOCK;
						}
						else {	
							symb = '/'; // previous parser
							if(ftell(fp) > 0)fseek(fp, -1, SEEK_CUR);
							token = new rc_token;
							token->type   = RC_PARSER;
							token->row_num = row_count;
							token->parser = symb; 
							table[token_count] = token;
							token_count++;
							
							if(this->bRCDebug){
								fprintf(fplog,"%d : PARSER = %c\n",row_count, (char)symb);
							}
							
							continue;
						}
					}// if symb == '/'				
				
					else if(symb == '.'){	// only float point value
						buf[0] = symb;
						buf_index = 1;
						scan_state = S_NUMVALUE;
					}

					else if(symb == '\"'){ // possible begin of string
						scan_state	= S_STRING;
						buf_index	= 0;	// reset index to begin
					}	
										
					else {	// simple parser 
						token = new rc_token;
						token->type   = RC_PARSER;
						token->row_num = row_count;
						token->parser = symb; 
						table[token_count] = token;
						token_count++;
						
						if(this->bRCDebug){
							fprintf(fplog,"%d : PARSER = %c\n",row_count, (char)symb);
						}
					}
					
					continue;
				}
				
				if(symb_class == CLASS_BAD_SYMBOL){ // unresolved symbol
					buf[buf_index] = symb;	 
					scan_state = S_ERROR;	// jump to error state
					buf_index++;
					continue; 
				}
			
				continue;				
			break; // S0
		
			case S_ID_RWORD:	// identifier or reserved word
				
				if(symb_class == CLASS_LETTER ||	// if got letter or digit
					symb_class == CLASS_DIGIT){
					
					buf[buf_index] = symb;			// save them to array and continue
					buf_index++;
					continue;
				}

				else if(symb_class == CLASS_PARSER ||
						symb == 0x0d ||
						symb == EOF){	// if parser or CRLF or EOF - id finished
					
					if(symb != EOF)
						fseek(fp, -1, SEEK_CUR);	// return symbol into the file
					scan_state = S0;			// jump to default state
					buf[buf_index] = '\0';		// complete string

					// initialize token by buf

					strToWCS(buf, (TCHAR*)wbuf);

					token		= new rc_token;		
					token->id	= new shString((const TCHAR*)wbuf);
					token->type = RC_IDENTIFIER;
					token->row_num = row_count;
					if(this->bRCDebug)
						fprintf(fplog,"%d : IDENTIFIER = %s\n",row_count,token->id->getCString());
										
					// add it to table
					table[token_count] = token;
					token_count++;
					buf_index = 0;
																		
					continue;
				}
					
				else { // error
					buf[buf_index] = symb;
					scan_state = S_ERROR;
					continue;
				}
				
				continue;
			break; // S_ID_RWORD
		
			case S_COMMENT_LINE:
				
				if(symb == EOF ||
					symb == 0x0d){
					
					if(symb != EOF)
						fseek(fp, -1, SEEK_CUR);
					scan_state = S0;
													
					continue;
				}	
							
				continue;
			break; // S_COMMENT_LINE
		
			case S_COMMENT_BLOCK:	// comment block state /*   */
			
				if(symb == EOF){
					
					scan_state  = S0; // jump to default state
					continue;
				
				}// if EOF
				
				else if(symb == 0x0d){ // if CRLF
					
					fgetc(fp);		// skip 0x0a
					row_count++;
				
				}// if CRLF
				
				else if(symb == '*'){ // may be comment end */
					
					symb = fgetc(fp);
					
					if(symb == EOF || symb == '/'){	// got end of comment

						scan_state  = S0; // jump to default state
						continue;

					}// if EOF
					
					else if(symb == 0x0d){ // if CRLF

						fgetc(fp);		// skip 0x0a
						row_count++;

					}// if CRLF		
					else fseek(fp, -1, SEEK_CUR);							
				
				}// if *
				

				continue;	
			break; // S_COMMENT_BLOCK
			
			case S_STRING:	// string value

				if(symb == '\"'){	// we'v got end of string
					buf[buf_index] = '\0';	// complete string
					
					// initialize token by string

					strToWCS(buf, (TCHAR*)wbuf);

					token = new rc_token;
					token->strval = new shString((const TCHAR*)wbuf);
					token->type = RC_STRING;					
					token->row_num = row_count;
					table[token_count] = token;
					token_count++;
					
					// jump to begin state
					buf_index = 0;
					scan_state = S0;
				
					if(this->bRCDebug)
						fprintf(fplog,"%d : STRING = %s\n",row_count,token->strval->getCString());
					
					continue;
				}

				// if got CRLF - fatal error 
				else if(symb == 0x0d || symb == EOF){
					
					// close all end exit
					fclose(fp);
					
					if(this->bRCDebug){
						if(symb == 0x0d)
							fprintf(fplog,"%d : FATAL ERROR! = unexpected end of line!\n",row_count);
						else 
							fprintf(fplog,"%d : FATAL ERROR! = unexpected end of file!\n",row_count);
						fclose(fplog);
					
					}
					
					*tb_size = token_count;

					return RC_ERRORSCANFILE;
				}
				
				else { // else continue scanning string
					buf[buf_index] = symb;
					buf_index++;
				}
				
				continue;	
			break; // S_STRING 

			case S_NUMVALUE:
				if(symb_class == CLASS_DIGIT || symb_class == CLASS_LETTER ||
					symb == '.' || symb == '+' || symb == '-'){ // continue scanning
					
					buf[buf_index] = symb;
					buf_index++;
					continue;
				}		
							
				else if(symb == ' ' ||	// we'v got end of num value
					symb == 0x09 ||
					symb == 0x0d ||
					symb == EOF ||
					symb_class == CLASS_PARSER){
				
					// return symbol into file
 					if(symb != EOF)fseek(fp,-1,SEEK_CUR);

					// initialize new token
					token = new rc_token;
					token->row_num = row_count;						
					
					buf[buf_index] = '\0';	// complete string
					
					// now we must determine value type 
					// example: 0x2233 or 0X2233 or 1.2f or 1.2F or 1.2 or 1.2e or 1.2E or 2l or 2L
					// ;)) i can determine type by another way but code will be not so readable
					for(i=0;i<buf_index;i++){
						// if hexadecimal value
						if(buf[i] == 'x' || buf[i] == 'X'){
							
							token->dval = strtoul(buf,&endptr,16);	// convert string to integer
							// check if warning
							if(endptr && endptr != &buf[buf_index] && *endptr != 'l' && *endptr != 'L'){
								if(this->bRCDebug){ // debug if needed
									fprintf(fplog,"%d : WARNING - not valid dword value. value will be set to 0x00 \n",row_count);
								}
							}
							else {
								if(this->bRCDebug){ // debug if needed
									fprintf(fplog,"%d : DWORD = %8.8x\n",row_count,token->dval);
								}
							}
							token->type = RC_DWORD;
																			
							break;
						}
						// if float value
						if(buf[i] == 'f' || buf[i] == 'F' ||
							buf[i] == 'e' || buf[i] == 'E' || buf[i] == '.'){
							
							token->fval = (float)strtod(buf,&endptr);
							if(endptr && endptr != &buf[buf_index] && *endptr != 'f' && *endptr != 'F'){ // some warnings
								if(this->bRCDebug){ // debug if needed
									fprintf(fplog,"%d : WARNING - not valid float value. value will be set to 0.0f \n",row_count);
								}
							}
							else {
								if(this->bRCDebug){ // debug if needed
									fprintf(fplog,"%d : FLOAT = %f\n",row_count,token->fval);
								}
							}
							token->type = RC_FLOAT;
							break;
						}
					}
					// if we didn't found thoose symbols - value is integer
					if(i == buf_index){
						
						token->dval = strtoul(buf,&endptr,10); // convert string to integer
						if(endptr && endptr != &buf[buf_index] && *endptr != 'l' && *endptr != 'L'){
							if(this->bRCDebug){ // debug if needed
								fprintf(fplog,"%d : WARNING - not valid integer value. value will be set to 0 \n",row_count);
							}
						}
						else {
							if(this->bRCDebug){ // debug if needed
								fprintf(fplog,"%d : INTEGER = %d\n",row_count,token->dval);
							}
						}
						token->type = RC_INT;
					}
					
					// now check if there is an + or - prev
					for(token_index = token_count-1; token_index >= 0; token_index--){
						if(table[token_index]->type == RC_PARSER){
							if(table[token_index]->parser == '+'){
								delete table[token_index];
							}
							else if(table[token_index]->parser == '-'){
								delete table[token_index];

								if(token->type == RC_FLOAT)token->fval = - token->fval;
								else if(token->type == RC_DWORD || token->type == RC_INT)token->dval = (-(int)(token->dval));
							}
							else break;
						}
						else break;
					}
					token_count = token_index + 1;						
										
					table[token_count] = token;
					token_count++;

					scan_state = S0;
					continue;
				}
								
				continue;	
			break; // S_NUMVALUE

		}
	}

	return RC_OK;
}

void* RCManager::GetElementByName(shString &CategoryName, RC_ITEM_TYPE ItemType, shString & ItemName, bool enable_msg){

	std::list< float >::iterator			fli;
	std::list< DwordValue >::iterator		di;
	std::list< shString* >::iterator		phi;
	std::list< shString >::iterator			namei;
	std::list<CCategoryValues*>::iterator	cvi;
		
	TCHAR buffer[512];	
	bool found;

	// if no last accessed category
	if(LastAccessedCategory == NULL || (*LastAccessedCategory) != CategoryName){
		found = false;
		cvi = CValues.begin();
		for(namei=Categories.begin(); namei!=Categories.end();namei++, cvi++){
			if(*namei == CategoryName){
				found = true;
				LastAccessedValues   = *cvi;
				LastAccessedCategory = &(*namei); 
				break;
			}
		}
		if(found == false){
			if(enable_msg){
				_tprintf(buffer,TEXT("Can't find category '%s'"),CategoryName);
				MessageBox(hWnd, buffer,TEXT("RCManager::GetElementByName(1)"),MB_ICONERROR|MB_OK);
			}
			return (void*)NULL;
		}
	}
		
	if(ItemType == RC_FLOAT){
		for(fli = LastAccessedValues->FloatValues.begin(), namei  = LastAccessedValues->FloatValueNames.begin() ; fli != LastAccessedValues->FloatValues.end(); fli++, namei++){
			if(*namei == ItemName){
				LastAccessedName = &(*namei);
				return (void*)&(*fli);	
			}
		}
		if(enable_msg){
			_tprintf(buffer,TEXT("Can't find resource type float '%s'"),ItemName.getCString());
			MessageBox(hWnd,buffer,TEXT("RCManager::GetElementByName(1)"),MB_ICONERROR|MB_OK);
		}
		return NULL;
	}
	
	else if(ItemType == RC_DWORD){
		for(di = LastAccessedValues->DwordValues.begin(), namei  = LastAccessedValues->DwordValueNames.begin() ; di != LastAccessedValues->DwordValues.end(); di++ ,namei++){
			if(*namei == ItemName){
				LastAccessedName = &(*namei);
				return (void*)&(*di);	
			}
		}
		if(enable_msg){
			_tprintf(buffer,TEXT("Can't find resource type DWORD '%s'"),ItemName.getCString());
			MessageBox(hWnd,buffer,TEXT("RCManager::GetElementByName(1)"),MB_ICONERROR|MB_OK);
		}
		return NULL;
	}
	
	else if(ItemType == RC_STRING){
		for(phi = LastAccessedValues->HStrings.begin(), namei  = LastAccessedValues->HStringNames.begin() ; phi != LastAccessedValues->HStrings.end(); phi++, namei++){
			if(*namei == ItemName){
				LastAccessedName = &(*namei);
				return (void*)(*phi);	
			}
		}
		if(enable_msg){
			_tprintf(buffer,TEXT("Can't find resource type shString '%s'"),ItemName.getCString());
			MessageBox(hWnd,buffer,TEXT("RCManager::GetElementByName(1)"),MB_ICONERROR|MB_OK);
		}
		return NULL;
	}
	return NULL;
}

void* RCManager::GetElementByName(TCHAR *CategoryName, RC_ITEM_TYPE ItemType,TCHAR *ItemName, bool enable_msg){

	shString HItemName(ItemName);
	shString HCategoryName(CategoryName);
	
	return GetElementByName(HCategoryName, ItemType, HItemName, enable_msg);

}

void* RCManager::GetElementByName(shString &CategoryName, RC_ITEM_TYPE ItemType,TCHAR *ItemName, bool enable_msg){

	shString HItemName(ItemName);
		
	return GetElementByName(CategoryName, ItemType, HItemName, enable_msg);

}

void* RCManager::GetElementByNameEx(shString &CategoryName, RC_ITEM_TYPE ItemType,shString &ItemName, shString **pItemName, bool enable_msg){

	void *ptr;
	
	ptr = GetElementByName(CategoryName, ItemType, ItemName, enable_msg);
	*pItemName = LastAccessedName;

	return ptr;
}

void* RCManager::GetElementByNameEx(TCHAR *CategoryName, RC_ITEM_TYPE ItemType,TCHAR *ItemName, shString **pItemName, bool enable_msg){

	shString hs(ItemName);
	shString hc(CategoryName);
	return GetElementByNameEx(hc, ItemType, hs, pItemName, enable_msg);
}

void* RCManager::GetCategory(TCHAR *CategoryName, bool enable_msg){
	
	shString HCategoryName(CategoryName);

	return GetCategory(HCategoryName, enable_msg);

}

void* RCManager::GetCategory(shString &CategoryName, bool enable_msg){
	
	std::list<CCategoryValues*>::iterator	cvi;
	std::list< shString >::iterator			namei;
	
	TCHAR	buffer[256];
	
	cvi = CValues.begin();
	for(namei=Categories.begin(); namei!=Categories.end();namei++, cvi++){
		if(*namei == CategoryName){
			LastAccessedValues   = *cvi;
			LastAccessedCategory = &(*namei); 
			return (*cvi);
		}
	}

	if(enable_msg){
		_tprintf(buffer,TEXT("Can't find category '%s'"),CategoryName.getCString());
		MessageBox(hWnd,buffer,TEXT("RCManager::GetCategory()"),MB_ICONERROR|MB_OK);	
	}
	
	return NULL;
}

void RCManager::AddCategory(shString &CatName, bool enable_msg){


	CCategoryValues *cat_vals;
	TCHAR			buffer[512];	
	
	typedef std::list<shString>::iterator SI;
	SI si;

	cat_vals = (CCategoryValues*)GetCategory(CatName, enable_msg);
	if(cat_vals == NULL){
		
		cat_vals = new CCategoryValues;
		CValues.push_back(cat_vals);
	
		Categories.push_back(CatName);
		
	}
	else {
		if(enable_msg){
			_tprintf(buffer,TEXT("Category '%s' already exists"),CatName.getCString());
			MessageBox(hWnd,buffer,TEXT("RCManager::AddCategory()"),MB_ICONERROR|MB_OK);	
		}
	}
}

void RCManager::AddCategory(TCHAR *CatName, bool enable_msg){

	shString sh(CatName);

	return AddCategory(sh, enable_msg);

}

void RCManager::AddValue(shString &ValName, DWORD Val, BYTE Type, bool enable_msg){
	
	TCHAR	buffer[512];
	DwordValue v;

	if(LastAccessedCategory == NULL)return ;
	DwordValue *TmpVal = (DwordValue*)GetElementByName(*LastAccessedCategory,RC_DWORD, ValName);	
	if(TmpVal != NULL){
		
		if(enable_msg){
			_tprintf(buffer,TEXT("DWORD Variable '%s' already exists"),ValName.getCString());
			MessageBox(hWnd,buffer,TEXT("RCManager::AddValue()"),MB_ICONERROR|MB_OK);	
		}
		
		TmpVal->Type = Type; 
		TmpVal->Val	 = Val;

	}
	else {
	
		v.Val = Val;
		v.Type = Type;
		LastAccessedValues->DwordValueNames.push_back(ValName);
		LastAccessedValues->DwordValues.push_back(v);
		
	}
}

void RCManager::AddValue(shString &ValName, float Val, bool enable_msg){
	
	TCHAR	buffer[512];
	
	if(LastAccessedCategory == NULL)return ;
	float *TmpVal = (float*)GetElementByName(*LastAccessedCategory,RC_FLOAT, ValName);	
	if(TmpVal != NULL){
		
		if(enable_msg){
			_tprintf(buffer,TEXT("float Variable '%s' already exists"),ValName.getCString());
			MessageBox(hWnd,buffer,TEXT("RCManager::AddValue()"),MB_ICONERROR|MB_OK);	
		}
		
		*TmpVal = Val;

	}
	else {
	
		LastAccessedValues->FloatValueNames.push_back(ValName);
		LastAccessedValues->FloatValues.push_back(Val);
		
	}
}

void RCManager::AddValue(shString &ValName, shString &Val, bool enable_msg){
	
	TCHAR	buffer[512];
	
	if(LastAccessedCategory == NULL)return ;
	shString *TmpVal = (shString*)GetElementByName(*LastAccessedCategory,RC_STRING, ValName);	
	if(TmpVal != NULL){
		
		if(enable_msg){
			_tprintf(buffer,TEXT("float Variable '%s' already exists"),ValName.getCString());
			MessageBox(hWnd,buffer,TEXT("RCManager::AddValue()"),MB_ICONERROR|MB_OK);	
		}
		
		*TmpVal = Val;

	}
	else {
	
		TmpVal = new shString(Val);
		LastAccessedValues->HStringNames.push_back(ValName);
		LastAccessedValues->HStrings.push_back(TmpVal);
		
	}
}

void RCManager::AddValue(TCHAR *ValName, DWORD Val, BYTE Type, bool enable_msg){

	shString sh(ValName);
	
	return AddValue(sh, Val, Type, enable_msg);

}

void RCManager::AddValue(TCHAR *ValName, float Val, bool enable_msg){

	shString sh(ValName);
	
	return AddValue(sh, Val, enable_msg);

}

void RCManager::AddValue(TCHAR *ValName, TCHAR *Val, bool enable_msg){

	shString sh(ValName);
	shString sh1(Val);

	return AddValue(sh, sh1, enable_msg);

}

RC_RETURN_CODE RCManager::ParseTable(rc_token **table, DWORD tb_size){	// parse table

	// we have next tokens
	// [ resource configuration ]
	// [ version 2.00 ] must be 2.00
	
	// [ category_name ]
	//		value_name	= all value types include string;
	//      value_name  = identifier;
	//		...
	// [ end category name ]

	int				token_index, rule_index;
	
	bool			file_header_found		= false;
	bool			version_found			= false;
	bool			in_section				= false;
		
	shString		*item_string;
	shString		cat_name;
	float			*item_float;
	DwordValue		*item_dword, dw1;
		
	rc_token		*rule[RULE_MAX_LEN];
	FILE			*fplog;
	
	TCHAR			buf[1024];
	
	CCategoryValues	*cvals;
	//shString		cat_name;
	
	if(this->bRCDebug){	// if required debug information 
		
		// generate log file name
		_stprintf_s(buf, 1024, TEXT("%s_pars_%3.3d_%3.3d.log"),FileName.getCString(),this->dwRCId,this->dwParseLogCount);
		_tfopen_s(&fplog, buf,TEXT("w"));		
		if(fplog == NULL){
			fclose(fplog);
			return RC_ERROROPENLOGFILE;
		}
		this->dwParseLogCount ++;	// increment log count
	}
	
	if(tb_size < 4)return RC_INVALIDCFGFILE;
	
	in_section = false;
	for(token_index = 0; token_index < (int)tb_size;){
	
		// get new parser rule
		for(rule_index = 0;rule_index < RULE_MAX_LEN, rule_index < (int)(tb_size - token_index) ;rule_index++){
			rule[rule_index] = table[token_index + rule_index];
			if(rule[rule_index]->type == RC_PARSER){
				if(rule[rule_index]->parser == ';' || rule[rule_index]->parser == ']')break;
			}
		}
		
		if( rule_index == RULE_MAX_LEN ||
			(rule[rule_index]->parser != ';' && rule[rule_index]->parser != ']')){
			if(bRCDebug){
					_ftprintf(fplog,TEXT("line %d -> expected ';' or ']'\n"),rule[rule_index]->row_num);
					return RC_ERRORPARSEFILE;
			}
		}
		
		rule_index++;
		token_index += rule_index;
				
		// we've got rule now check it
		
		// [ resource configuration ]
		if(rule_index == 4 && 
			rule[0]->type == RC_PARSER && rule[0]->parser == '[' &&
			rule[1]->type == RC_IDENTIFIER && *(rule[1]->strval) == TEXT("resource") &&
			rule[2]->type == RC_IDENTIFIER && *(rule[2]->strval) == TEXT("configuration") &&
			rule[3]->type == RC_PARSER && rule[3]->parser == ']'
			){
			
			if(bRCDebug)
				_ftprintf(fplog,TEXT("line %d -> found file header ( [ resource configuration ] )\n"),rule[3]->row_num);
						
			if(file_header_found){
				if(bRCDebug)
					_ftprintf(fplog,TEXT("line %d -> WARNING: found duplicate file header ( [ resource configuration ] )\n"),rule[3]->row_num);	
			}
			else 
				file_header_found = true;
			
			continue ;
		}// [ resource configuration ]
	
		// [ version 2.00 ] must be 2.00
		else if(rule_index == 4 && 
			rule[0]->type == RC_PARSER && rule[0]->parser == '[' &&
			rule[1]->type == RC_IDENTIFIER && *(rule[1]->strval) == TEXT("version") &&
			rule[2]->type == RC_FLOAT && rule[2]->fval == 2.00f &&
			rule[3]->type == RC_PARSER && rule[3]->parser == ']'){
			
			if( !file_header_found ){
				if(bRCDebug){
					_ftprintf(fplog,TEXT("ERROR: THIS IS NOT RESOURCE CONFIGURATION FILE FOR 'RCManager' CLASS\n"));	
					fclose(fplog);
				}
				return RC_ERRORPARSEFILE;
			}
			else if(version_found){
				if(bRCDebug)
					_ftprintf(fplog,TEXT("line %d -> WARNING: found duplicate file version ( [ version 2.00 ] )\n"),rule[3]->row_num);		
			}	
			else {
				version_found = true;
				if(bRCDebug)
					_ftprintf(fplog,TEXT("line %d -> found file version ( [ version 1.00 ] )\n"),rule[3]->row_num);		
			}	
			
			continue;
		}// [ version 2.00 ] must be 2.00 
		
		// this is another rule
		// check if we can parse file
		if(!file_header_found){
			if(bRCDebug){
				_ftprintf(fplog,TEXT("ERROR: THIS IS NOT RESOURCE CONFIGURATION FILE FOR 'RCManager' CLASS\n"));	
				fclose(fplog);
			}
			return RC_ERRORPARSEFILE;
		}
		else if(!version_found){
			if(bRCDebug){
				_ftprintf(fplog,TEXT("ERROR: THIS IS NOT VALID RESOURCE CONFIGURATION FILE VERSION FOR 'RCManager' CLASS\n"));	
				fclose(fplog);
			}
			return RC_ERRORPARSEFILE;
		}	
		
	
		// [ category_name ]
		//		value_name	= all value types include string;
		//      value_name  = identifier;
		//		...
		// [ end category_name ]	
		

		// [ category_name ]
		else if(rule_index == 3 && 
			rule[0]->type == RC_PARSER && rule[0]->parser == '[' &&
			rule[1]->type == RC_IDENTIFIER &&
			rule[2]->type == RC_PARSER && rule[2]->parser == ']'
			){
		
			if( in_section ){ // in other section
				if(bRCDebug){
					_ftprintf(fplog,TEXT("line %d -> error: declare new_category section in category section (you must end prev. section to declare new)\n"),rule[2]->row_num);	
					fclose(fplog);
				}
				return RC_ERRORPARSEFILE;
			}	
					
			// all ok
			in_section			= true;
			
			// now we must check if category exist
			cvals = (CCategoryValues*)GetCategory(*(rule[1]->strval));
			if(cvals == NULL){ // no such category
				cvals    = new CCategoryValues;
				cat_name = *(rule[1]->strval); 
				CValues.push_back(cvals);
				Categories.push_back(*(rule[1]->strval));
			}
			
			cat_name = *rule[1]->strval;

			if(bRCDebug){
				_ftprintf(fplog, TEXT("Found new category %s\n"), rule[1]->strval->getCString());
			}

			continue;
		}// [ category_name ]

		// value_name			= 0; optional
		else if(rule_index == 4 &&
			rule[0]->type == RC_IDENTIFIER &&
			rule[1]->type == RC_PARSER && rule[1]->parser == '=' &&
			rule[3]->type == RC_PARSER && rule[3]->parser == ';'){
			
			if(!in_section){
				if(bRCDebug){
					_ftprintf(fplog,TEXT("line %d -> warning: ignore 'value' definition outside value section.\n"),rule[3]->row_num);			
				}
				continue ;
			}
				
			if(rule[2]->type == RC_DWORD || rule[2]->type == RC_INT){
				item_dword = (DwordValue*)GetElementByName(cat_name, RC_DWORD,*(rule[0]->strval));
				if(item_dword != NULL){
					if(rule[2]->type == RC_DWORD)item_dword->Type = 16;
					else item_dword->Type = 10;
					
					item_dword->Val = rule[2]->dval;
					if(bRCDebug){
						if(rule[2]->type == RC_DWORD)
							_ftprintf(fplog,TEXT("line %d -> warning: dword '%s' value already declard. new value = 0x%8.8x (dword)\n"),rule[3]->row_num,rule[0]->strval->getCString(),item_dword->Val);				
						else 
							_ftprintf(fplog,TEXT("line %d -> warning: dword '%s' value already declard. new value = %d (int)\n"),rule[3]->row_num,rule[0]->strval->getCString(),item_dword->Val);				
					}
				}
				else{
					if(bRCDebug){
						if(rule[2]->type == RC_DWORD)
							_ftprintf(fplog,TEXT("line %d -> value %s = 0x%8.8x (dword);\n"),rule[3]->row_num,rule[0]->strval->getCString(),rule[2]->dval);				
						else 
							_ftprintf(fplog,TEXT("line %d -> value %s = %d (int);\n"),rule[3]->row_num,rule[0]->strval->getCString(),rule[2]->dval);				
					}
					
					if(rule[2]->type == RC_DWORD)
						dw1.Type = 16;
					else 
						dw1.Type = 10;
					dw1.Val = rule[2]->dval;
					cvals->DwordValues.push_back(dw1);
					cvals->DwordValueNames.push_back(*(rule[0]->strval));
				}
			}
			else if(rule[2]->type == RC_FLOAT){
				item_float = (float*)GetElementByName(cat_name, RC_FLOAT,*(rule[0]->strval));
				if(item_float != NULL){
					*item_float = rule[2]->fval;
					if(bRCDebug){
						_ftprintf(fplog,TEXT("line %d -> warning: float '%s' value already declard. new value = %f (float)\n"),rule[3]->row_num,rule[0]->strval->getCString(),*item_float);				
					}
				}
				else{
					if(bRCDebug){
						_ftprintf(fplog,TEXT("line %d -> value %s = %f (float) ;\n"),rule[3]->row_num,rule[0]->strval->getCString(),rule[2]->fval);				
					}
					cvals->FloatValues.push_back(rule[2]->fval);
					cvals->FloatValueNames.push_back(*(rule[0]->strval));
				}
			}			
			
			else if(rule[2]->type == RC_STRING){
				item_string = (shString*)GetElementByName(cat_name, RC_STRING,*(rule[0]->strval));
				if(item_string != NULL){
					(*item_string) = *(rule[2]->strval);
					if(bRCDebug){
						_ftprintf(fplog,TEXT("line %d -> warning: string '%s' value already declard. new value = \"%s\" (string)\n"),rule[3]->row_num,rule[0]->strval->getCString(),item_string->getCString());				
					}
					//delete[] rule[3]->strval;
				}
				else{
					if(bRCDebug){
						_ftprintf(fplog,TEXT("line %d -> value %s = \"%s\" (string) ;\n"),rule[3]->row_num,rule[0]->strval->getCString(),rule[2]->strval->getCString());				
					}
					item_string = new shString;
					item_string->copy(*(rule[2]->strval));
					cvals->HStrings.push_back(item_string);
					cvals->HStringNames.push_back(*(rule[0]->strval));
				}
			}			
			
			else {
				if(bRCDebug){
					_ftprintf(fplog,TEXT("line %d -> WARNING: UNRESOLVED COMMAND!!! SKIPPED!!!\n"),rule[rule_index-1]->row_num);
				}
			}

			continue;
		}
		
		//	[ end catname]
		else if(rule_index == 4 &&
			rule[0]->type == RC_PARSER && rule[0]->parser == '[' &&
			rule[1]->type == RC_IDENTIFIER && *(rule[1]->strval) == TEXT("end") &&
			rule[3]->type == RC_PARSER && rule[3]->parser == ']'){
			
			/*
			if(!in_values_section){
				if(bRCDebug){
					_ftprintf(fplog,TEXT("line %d -> warning: ignore '[ end values ]' command outside values section.\n"),rule[3]->row_num);			
				}
				continue ;
			}
			*/
			in_section = false;
			
			if(cat_name != *rule[2]->strval){
				if(bRCDebug){
					_ftprintf(fplog, TEXT("ERROR: [end category %s] != %s\n"), cat_name.getCString(), rule[2]->strval->getCString());
				}
			}
			else {
				if(bRCDebug){
					_ftprintf(fplog, TEXT("end category %s\n"), cat_name.getCString());
				}
			}
			//in_values_section = false;
						
			continue;
		}//	[ end values ]
	
		else { // no command
			if(bRCDebug){
				_ftprintf(fplog,TEXT("line %d -> WARNING: UNRESOLVED COMMAND!!! SKIPPED!!!\n"),rule[rule_index-1]->row_num);
			}
		}	

	}// for

	if(bRCDebug){
		fclose(fplog);
	}
	
	LastAccessedCategory = NULL;
	LastAccessedValues = NULL;

	return RC_OK;
}
	
// load resourse set from file		
RC_RETURN_CODE RCManager::Load(TCHAR *filename){
	
	rc_token		**table;
	RC_RETURN_CODE	ret_code;
	DWORD			table_size;
	
	// add standart information;
	shString	app_path;
	shString	app_path_name(TEXT("ApplicationPath"));	
	shString	app_file;
	shString	app_file_name(TEXT("ApplicationName"));
	
	TCHAR filepathname[2048];
	TCHAR full_path[2048];
	TCHAR *app_filename;

	GetModuleFileName(NULL,filepathname,2048);	
	GetFullPathName(filepathname,2048,full_path,&app_filename);
	
	app_file.initialize(app_filename);
		
	AddCategory(TEXT("SYSTEM"));
	SelectCategory(TEXT("SYSTEM"));
	AddValue(app_file_name, app_file);
	*app_filename = '\0';
	app_path.initialize(full_path);
	AddValue(app_path_name, app_path);
	
	// allocate memory for token table
	table = new rc_token*[ 100000 ];
	if(table == NULL)
		return RC_ERRORALLOCATEMEM;
	
	ZeroMemory(table, 100000 * sizeof(rc_token*));	
	
	// scan file and get tokens
	ret_code = this->ScanFile(filename,table,&table_size);
	if(ret_code != RC_OK){
		delete[] table;
		return ret_code;
	}
		
	ret_code = this->ParseTable(table,table_size);	
	if(ret_code != RC_OK){
		delete[] table;
		return ret_code;
	}
	
	int i;
	for(i=0;i<100000; i++){
		if(table[i] != NULL){
			if(table[i]->type == RC_STRING && table[i]->strval != NULL)delete table[i]->strval;
			if(table[i]->type == RC_IDENTIFIER && table[i]->id != NULL)delete table[i]->id;
			delete table[i];
		}
		
	}
	delete[] table;

	return RC_OK;
}

RC_RETURN_CODE RCManager::Save(TCHAR *filename){

	// сохраняем в файл	
	FILE *fp;
	
	shString sys_str(TEXT("SYSTEM"));
	
	_tfopen_s(&fp, filename,TEXT("w"));
	
	typedef std::list<shString >::iterator SI; 
	SI si, si1;
	typedef std::list<shString *>::iterator SI2; 
	SI2 si2;
	
	typedef std::list<DwordValue>::iterator DI; 
	DI di;

	typedef std::list<float >::iterator FI; 
	FI fi;
	
	typedef std::list<CCategoryValues *>::iterator CI; 
	CI ci;
	
	CCategoryValues *cvals;
	
	// [ resource configuration ]
	// [ version 2.00 ] must be 2.00
	_ftprintf(fp, TEXT("[ resource configuration ]\n"));
	_ftprintf(fp, TEXT("[ version 2.00 ]\n\n"));
	
	for(ci = CValues.begin(), si=Categories.begin(); si != Categories.end(); si++, ci++){
		// печать начала категории
		if(si->isEqual(sys_str))continue;
		_ftprintf(fp, TEXT("[ %s ]\n"), si->getCString());

		cvals = *ci;
		// печатаем все элементы категории по порядку
		// строки
		if(cvals->HStrings.begin() != cvals->HStrings.end()) _ftprintf(fp, TEXT("\n"));
		for(si2 = cvals->HStrings.begin(), si1 = cvals->HStringNames.begin(); si2 != cvals->HStrings.end(); si1++, si2++){
			_ftprintf(fp, TEXT("\t%s = \"%s\";\n"), si1->getCString(), (*si2)->getCString());
		}
		
		// целые значения
		if(cvals->DwordValues.begin() != cvals->DwordValues.end()) _ftprintf(fp, TEXT("\n"));
		for(si1 = cvals->DwordValueNames.begin(), di = cvals->DwordValues.begin(); si1 != cvals->DwordValueNames.end(); si1++, di++){
			if(di->Type == 16)
				_ftprintf(fp, TEXT("\t%s = 0x%8.8x;\n"), si1->getCString(), di->Val);
			else 
				_ftprintf(fp, TEXT("\t%s = %d;\n"), si1->getCString(), (int)di->Val);
		}
		
		// вещественные значения
		if(cvals->FloatValues.begin() != cvals->FloatValues.end()) _ftprintf(fp, TEXT("\n"));
		for(si1 = cvals->FloatValueNames.begin(), fi = cvals->FloatValues.begin(); si1 != cvals->FloatValueNames.end(); si1++, fi++){
			_ftprintf(fp, TEXT("\t%s = %f;\n"), si1->getCString(), *fi);
		}
		
		// печать начала категории
		_ftprintf(fp, TEXT("\n[ end %s ]\n\n"), si->getCString());
	}
	
	fclose(fp);
	return RC_OK;
}

void RCManager::Clear(){

	typedef std::list<CCategoryValues *>::iterator CI; 
	CI ci;
	
	for(ci = CValues.begin(); ci != CValues.end(); ci++){
		
		(*ci)->HStrings.clear();
		(*ci)->HStringNames.clear();

		(*ci)->DwordValues.clear();
		(*ci)->DwordValueNames.clear();

		(*ci)->FloatValues.clear();
		(*ci)->FloatValueNames.clear();
	
		delete *ci; 
	}

	Categories.clear();
	CValues.clear();

}