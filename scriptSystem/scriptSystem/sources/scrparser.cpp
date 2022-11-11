#include "scrparser.h"
#include "symparser.h"

scriptRetCode scriptParser::parse( scriptScaner *sc ) {

    this->tabCount = 0;
    this->tab[0] = 0;
    this->scaner = sc;
    this->sli = this->scaner->getLexemesList().begin();

    this->symParser = new scriptSymantecParser(this);
    if( this->sli == this->scaner->getLexemesList().end() )
        return SRV_SUCCESS;

    if( sli == this->scaner->getLexemesList().end() )
        _tprintf(TEXT("end of file.\n"));
    else
        _tprintf(TEXT("%s\n"), (*sli)->getLexemeName().getCString());
    scriptRetCode res = this->enter_rule();
    if(res == SRV_SUCCESS || res == SPRV_EOF)
        return this->OnSuccess_enter_rule();
    if(res == SRV_FAIL)
        return this->OnFail_enter_rule();
    return res;
}

void scriptParser::addTab() {
    this->tab[this->tabCount] = '\t';
    this->tab[this->tabCount + 1] = 0;
    this->tabCount++;
}

void scriptParser::removeTab() {
    if(this->tabCount == 0) return;
    this->tab[this->tabCount - 1] = 0;
    this->tabCount--;
}

scriptRetCode scriptParser::isRword( lexRWordType rword ) {

    if( sli == this->scaner->getLexemesList().end() )
        return SPRV_EOF;
    if( (*sli)->type != LT_RWORD || (*sli)->rwordType != rword )
        return SPRV_NOT_THIS_RULE;
    sli++;
    if( sli == this->scaner->getLexemesList().end() )
        _tprintf(TEXT("%sEOF.\n"), this->tab);
    else
        _tprintf(TEXT("%s%s\n"), this->tab, (*sli)->getLexemeName().getCString());
    return SRV_SUCCESS;
}

scriptRetCode scriptParser::isParser( lexParserType parser ) {

    if( sli == this->scaner->getLexemesList().end() )
        return SPRV_EOF;
    if( (*sli)->type != LT_PARSER || (*sli)->parser != parser )
        return SPRV_NOT_THIS_RULE;
    sli++;
    if( sli == this->scaner->getLexemesList().end() )
        _tprintf(TEXT("%sEOF\n"), this->tab);
    else
        _tprintf(TEXT("%s%s\n"), this->tab, (*sli)->getLexemeName().getCString());
    return SRV_SUCCESS;
}

scriptRetCode scriptParser::isConstValue() {

    if( sli == this->scaner->getLexemesList().end() )
        return SPRV_EOF;
    if( (*sli)->type != LT_CONST )
        return SPRV_NOT_THIS_RULE;
    if( sli == this->scaner->getLexemesList().end() )
        _tprintf(TEXT("%sEOF\n"), this->tab);
    else
        _tprintf(TEXT("%s%s\n"), this->tab, (*sli)->getLexemeName().getCString());
    sli++;
    return SRV_SUCCESS;
}

scriptRetCode scriptParser::isId() {

    if( sli == this->scaner->getLexemesList().end() )
        return SPRV_EOF;
    if( (*sli)->type != LT_ID )
        return SPRV_NOT_THIS_RULE;
    sli++;
    if( sli == this->scaner->getLexemesList().end() )
        _tprintf(TEXT("%sEOF\n"), this->tab);
    else
        _tprintf(TEXT("%s%s\n"), this->tab, (*sli)->getLexemeName().getCString());
    return SRV_SUCCESS;
}

// enter_rule :: = Programm ; 
scriptRetCode scriptParser::enter_rule() {

    scriptRetCode res;

    this->addTab();
    _tprintf(TEXT("%schecking enter_rule_part0\n"), this->tab);
    res = this->enter_rule_part0();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_enter_rule_part0();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_enter_rule_part0();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
    }
    _tprintf(TEXT("%snot this rule.\n"), this->tab);
    this->removeTab();
    return SPRV_NOT_THIS_RULE;
}

// Programm 
scriptRetCode scriptParser::enter_rule_part0() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
    _tprintf(TEXT("%schecking Programm\n"), this->tab);
    res = this->Programm();
    if(res == SRV_SUCCESS) {
        finalRes = SRV_SUCCESS;
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        res = this->OnSuccess_Programm();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        res = this->OnFail_Programm();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SPRV_NOT_THIS_RULE) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        _tprintf(TEXT("%snot this rule.\n"), this->tab);
        return SPRV_NOT_THIS_RULE;
    }
    else if(res == SPRV_EOF) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        this->removeTab();
        return SPRV_EOF;
    }
    else if(res != SPRV_CAN_CONTINUE) {
        this->removeTab();
        return res;
    }

    this->removeTab();
    return finalRes;
}

// Programm :: = [ UsingDefinition ] [ NamespaceDefinition ] ; 
scriptRetCode scriptParser::Programm() {

    scriptRetCode res;

    this->addTab();
    _tprintf(TEXT("%schecking Programm_part0\n"), this->tab);
    res = this->Programm_part0();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_Programm_part0();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_Programm_part0();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
    }
    _tprintf(TEXT("%snot this rule.\n"), this->tab);
    this->removeTab();
    return SPRV_NOT_THIS_RULE;
}

// [ UsingDefinition ] [ NamespaceDefinition ] 
scriptRetCode scriptParser::Programm_part0() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
    _tprintf(TEXT("%schecking Programm_part0_inplace0\n"), this->tab);
    res = this->Programm_part0_inplace0();
    if(res == SRV_SUCCESS) {
        finalRes = SRV_SUCCESS;
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        res = this->OnSuccess_Programm_part0_inplace0();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        res = this->OnFail_Programm_part0_inplace0();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SPRV_NOT_THIS_RULE) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        _tprintf(TEXT("%snot this rule.\n"), this->tab);
        return SPRV_NOT_THIS_RULE;
    }
    else if(res == SPRV_EOF) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        this->removeTab();
        return SPRV_EOF;
    }
    else if(res != SPRV_CAN_CONTINUE) {
        this->removeTab();
        return res;
    }
    _tprintf(TEXT("%schecking Programm_part0_inplace1\n"), this->tab);
    res = this->Programm_part0_inplace1();
    if(res == SRV_SUCCESS) {
        finalRes = SRV_SUCCESS;
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        res = this->OnSuccess_Programm_part0_inplace1();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        res = this->OnFail_Programm_part0_inplace1();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SPRV_NOT_THIS_RULE) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        _tprintf(TEXT("%snot this rule.\n"), this->tab);
        return SPRV_NOT_THIS_RULE;
    }
    else if(res == SPRV_EOF) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        this->removeTab();
        return SPRV_EOF;
    }
    else if(res != SPRV_CAN_CONTINUE) {
        this->removeTab();
        return res;
    }

    this->removeTab();
    return finalRes;
}

// [ UsingDefinition ] 
scriptRetCode scriptParser::Programm_part0_inplace0() {

    scriptRetCode res;

    this->addTab();
    _tprintf(TEXT("%schecking Programm_part0_inplace0_part0\n"), this->tab);
    res = this->Programm_part0_inplace0_part0();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_Programm_part0_inplace0_part0();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_Programm_part0_inplace0_part0();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
        return res;
    }
    _tprintf(TEXT("%scan continue.\n"), this->tab);
    this->removeTab();
    return SPRV_CAN_CONTINUE;
}

// UsingDefinition 
scriptRetCode scriptParser::Programm_part0_inplace0_part0() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
    _tprintf(TEXT("%schecking UsingDefinition\n"), this->tab);
    res = this->UsingDefinition();
    if(res == SRV_SUCCESS) {
        finalRes = SRV_SUCCESS;
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        res = this->OnSuccess_UsingDefinition();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        res = this->OnFail_UsingDefinition();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SPRV_NOT_THIS_RULE) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        _tprintf(TEXT("%snot this rule.\n"), this->tab);
        return SPRV_NOT_THIS_RULE;
    }
    else if(res == SPRV_EOF) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        this->removeTab();
        return SPRV_EOF;
    }
    else if(res != SPRV_CAN_CONTINUE) {
        this->removeTab();
        return res;
    }

    this->removeTab();
    return finalRes;
}

// [ NamespaceDefinition ] 
scriptRetCode scriptParser::Programm_part0_inplace1() {

    scriptRetCode res;

    this->addTab();
    _tprintf(TEXT("%schecking Programm_part0_inplace1_part0\n"), this->tab);
    res = this->Programm_part0_inplace1_part0();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_Programm_part0_inplace1_part0();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_Programm_part0_inplace1_part0();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
        return res;
    }
    _tprintf(TEXT("%scan continue.\n"), this->tab);
    this->removeTab();
    return SPRV_CAN_CONTINUE;
}

// NamespaceDefinition 
scriptRetCode scriptParser::Programm_part0_inplace1_part0() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
    _tprintf(TEXT("%schecking NamespaceDefinition\n"), this->tab);
    res = this->NamespaceDefinition();
    if(res == SRV_SUCCESS) {
        finalRes = SRV_SUCCESS;
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        res = this->OnSuccess_NamespaceDefinition();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        res = this->OnFail_NamespaceDefinition();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SPRV_NOT_THIS_RULE) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        _tprintf(TEXT("%snot this rule.\n"), this->tab);
        return SPRV_NOT_THIS_RULE;
    }
    else if(res == SPRV_EOF) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        this->removeTab();
        return SPRV_EOF;
    }
    else if(res != SPRV_CAN_CONTINUE) {
        this->removeTab();
        return res;
    }

    this->removeTab();
    return finalRes;
}

// UsingDefinition :: = { using QualifiedTypeName ";" } ; 
scriptRetCode scriptParser::UsingDefinition() {

    scriptRetCode res;

    this->addTab();
    _tprintf(TEXT("%schecking UsingDefinition_part0\n"), this->tab);
    res = this->UsingDefinition_part0();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_UsingDefinition_part0();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_UsingDefinition_part0();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
    }
    _tprintf(TEXT("%snot this rule.\n"), this->tab);
    this->removeTab();
    return SPRV_NOT_THIS_RULE;
}

// { using QualifiedTypeName ";" } 
scriptRetCode scriptParser::UsingDefinition_part0() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
    _tprintf(TEXT("%schecking UsingDefinition_part0_inplace0\n"), this->tab);
    res = this->UsingDefinition_part0_inplace0();
    if(res == SRV_SUCCESS) {
        finalRes = SRV_SUCCESS;
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        res = this->OnSuccess_UsingDefinition_part0_inplace0();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        res = this->OnFail_UsingDefinition_part0_inplace0();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SPRV_NOT_THIS_RULE) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        _tprintf(TEXT("%snot this rule.\n"), this->tab);
        return SPRV_NOT_THIS_RULE;
    }
    else if(res == SPRV_EOF) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        this->removeTab();
        return SPRV_EOF;
    }
    else if(res != SPRV_CAN_CONTINUE) {
        this->removeTab();
        return res;
    }

    this->removeTab();
    return finalRes;
}

// { using QualifiedTypeName ";" } 
scriptRetCode scriptParser::UsingDefinition_part0_inplace0() {

    scriptRetCode res;

    bool firstPart = true;
    this->addTab();
    while( true ) {
        _tprintf(TEXT("%schecking UsingDefinition_part0_inplace0_part0\n"), this->tab);
        res = this->UsingDefinition_part0_inplace0_part0();
        if(res == SRV_SUCCESS) {
            _tprintf(TEXT("%ssuccess.\n"), this->tab);
            this->OnSuccess_UsingDefinition_part0_inplace0_part0();
            firstPart = false;
            continue;
        }
        else if(res == SRV_FAIL) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return this->OnFail_UsingDefinition_part0_inplace0_part0();
        }
        else if(res != SPRV_NOT_THIS_RULE) {
            _tprintf(TEXT("%sanother res.\n"), this->tab);
            this->removeTab();
            return res;
        }
        break;
    }
    _tprintf(firstPart? TEXT("%scan continue.\n"): TEXT("%ssuccess.\n"), this->tab);
    this->removeTab();
    return firstPart? SPRV_CAN_CONTINUE: SRV_SUCCESS;
}

// using QualifiedTypeName ";" 
scriptRetCode scriptParser::UsingDefinition_part0_inplace0_part0() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
	res = this->isRword( LRW_USING );
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }
    _tprintf(TEXT("%schecking QualifiedTypeName\n"), this->tab);
    res = this->QualifiedTypeName();
    if(res == SRV_SUCCESS) {
        finalRes = SRV_SUCCESS;
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        res = this->OnSuccess_QualifiedTypeName();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        res = this->OnFail_QualifiedTypeName();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SPRV_NOT_THIS_RULE) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        _tprintf(TEXT("%snot this rule.\n"), this->tab);
        return SPRV_NOT_THIS_RULE;
    }
    else if(res == SPRV_EOF) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        this->removeTab();
        return SPRV_EOF;
    }
    else if(res != SPRV_CAN_CONTINUE) {
        this->removeTab();
        return res;
    }
    res = this->isParser( LPT_DOTCOMMA );
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }

    this->removeTab();
    return finalRes;
}

// NamespaceDefinition :: = NamespaceDefinitionBegin "{" NamespaceMembersDefinition "}" ; 
scriptRetCode scriptParser::NamespaceDefinition() {

    scriptRetCode res;

    this->addTab();
    _tprintf(TEXT("%schecking NamespaceDefinition_part0\n"), this->tab);
    res = this->NamespaceDefinition_part0();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_NamespaceDefinition_part0();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_NamespaceDefinition_part0();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
    }
    _tprintf(TEXT("%snot this rule.\n"), this->tab);
    this->removeTab();
    return SPRV_NOT_THIS_RULE;
}

// NamespaceDefinitionBegin "{" NamespaceMembersDefinition "}" 
scriptRetCode scriptParser::NamespaceDefinition_part0() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
    _tprintf(TEXT("%schecking NamespaceDefinitionBegin\n"), this->tab);
    res = this->NamespaceDefinitionBegin();
    if(res == SRV_SUCCESS) {
        finalRes = SRV_SUCCESS;
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        res = this->OnSuccess_NamespaceDefinitionBegin();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        res = this->OnFail_NamespaceDefinitionBegin();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SPRV_NOT_THIS_RULE) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        _tprintf(TEXT("%snot this rule.\n"), this->tab);
        return SPRV_NOT_THIS_RULE;
    }
    else if(res == SPRV_EOF) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        this->removeTab();
        return SPRV_EOF;
    }
    else if(res != SPRV_CAN_CONTINUE) {
        this->removeTab();
        return res;
    }
    res = this->isParser( LPT_LEFT_FIGURE_BRACKET );
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }
    _tprintf(TEXT("%schecking NamespaceMembersDefinition\n"), this->tab);
    res = this->NamespaceMembersDefinition();
    if(res == SRV_SUCCESS) {
        finalRes = SRV_SUCCESS;
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        res = this->OnSuccess_NamespaceMembersDefinition();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        res = this->OnFail_NamespaceMembersDefinition();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SPRV_NOT_THIS_RULE) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        _tprintf(TEXT("%snot this rule.\n"), this->tab);
        return SPRV_NOT_THIS_RULE;
    }
    else if(res == SPRV_EOF) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        this->removeTab();
        return SPRV_EOF;
    }
    else if(res != SPRV_CAN_CONTINUE) {
        this->removeTab();
        return res;
    }
    res = this->isParser( LPT_RIGHT_FIGURE_BRACKET );
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }

    this->removeTab();
    return finalRes;
}

// NamespaceDefinitionBegin :: = namespace QualifiedTypeName ; 
scriptRetCode scriptParser::NamespaceDefinitionBegin() {

    scriptRetCode res;

    this->addTab();
    _tprintf(TEXT("%schecking NamespaceDefinitionBegin_part0\n"), this->tab);
    res = this->NamespaceDefinitionBegin_part0();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_NamespaceDefinitionBegin_part0();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_NamespaceDefinitionBegin_part0();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
    }
    _tprintf(TEXT("%snot this rule.\n"), this->tab);
    this->removeTab();
    return SPRV_NOT_THIS_RULE;
}

// namespace QualifiedTypeName 
scriptRetCode scriptParser::NamespaceDefinitionBegin_part0() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
	res = this->isRword( LRW_NAMESPACE );
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }
    _tprintf(TEXT("%schecking QualifiedTypeName\n"), this->tab);
    res = this->QualifiedTypeName();
    if(res == SRV_SUCCESS) {
        finalRes = SRV_SUCCESS;
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        res = this->OnSuccess_QualifiedTypeName();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        res = this->OnFail_QualifiedTypeName();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SPRV_NOT_THIS_RULE) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        _tprintf(TEXT("%snot this rule.\n"), this->tab);
        return SPRV_NOT_THIS_RULE;
    }
    else if(res == SPRV_EOF) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        this->removeTab();
        return SPRV_EOF;
    }
    else if(res != SPRV_CAN_CONTINUE) {
        this->removeTab();
        return res;
    }

    this->removeTab();
    return finalRes;
}

// NamespaceMembersDefinition :: = { NamespaceMemberDefinition } ; 
scriptRetCode scriptParser::NamespaceMembersDefinition() {

    scriptRetCode res;

    this->addTab();
    _tprintf(TEXT("%schecking NamespaceMembersDefinition_part0\n"), this->tab);
    res = this->NamespaceMembersDefinition_part0();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_NamespaceMembersDefinition_part0();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_NamespaceMembersDefinition_part0();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
    }
    _tprintf(TEXT("%snot this rule.\n"), this->tab);
    this->removeTab();
    return SPRV_NOT_THIS_RULE;
}

// { NamespaceMemberDefinition } 
scriptRetCode scriptParser::NamespaceMembersDefinition_part0() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
    _tprintf(TEXT("%schecking NamespaceMembersDefinition_part0_inplace0\n"), this->tab);
    res = this->NamespaceMembersDefinition_part0_inplace0();
    if(res == SRV_SUCCESS) {
        finalRes = SRV_SUCCESS;
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        res = this->OnSuccess_NamespaceMembersDefinition_part0_inplace0();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        res = this->OnFail_NamespaceMembersDefinition_part0_inplace0();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SPRV_NOT_THIS_RULE) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        _tprintf(TEXT("%snot this rule.\n"), this->tab);
        return SPRV_NOT_THIS_RULE;
    }
    else if(res == SPRV_EOF) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        this->removeTab();
        return SPRV_EOF;
    }
    else if(res != SPRV_CAN_CONTINUE) {
        this->removeTab();
        return res;
    }

    this->removeTab();
    return finalRes;
}

// { NamespaceMemberDefinition } 
scriptRetCode scriptParser::NamespaceMembersDefinition_part0_inplace0() {

    scriptRetCode res;

    bool firstPart = true;
    this->addTab();
    while( true ) {
        _tprintf(TEXT("%schecking NamespaceMembersDefinition_part0_inplace0_part0\n"), this->tab);
        res = this->NamespaceMembersDefinition_part0_inplace0_part0();
        if(res == SRV_SUCCESS) {
            _tprintf(TEXT("%ssuccess.\n"), this->tab);
            this->OnSuccess_NamespaceMembersDefinition_part0_inplace0_part0();
            firstPart = false;
            continue;
        }
        else if(res == SRV_FAIL) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return this->OnFail_NamespaceMembersDefinition_part0_inplace0_part0();
        }
        else if(res != SPRV_NOT_THIS_RULE) {
            _tprintf(TEXT("%sanother res.\n"), this->tab);
            this->removeTab();
            return res;
        }
        break;
    }
    _tprintf(firstPart? TEXT("%scan continue.\n"): TEXT("%ssuccess.\n"), this->tab);
    this->removeTab();
    return firstPart? SPRV_CAN_CONTINUE: SRV_SUCCESS;
}

// NamespaceMemberDefinition 
scriptRetCode scriptParser::NamespaceMembersDefinition_part0_inplace0_part0() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
    _tprintf(TEXT("%schecking NamespaceMemberDefinition\n"), this->tab);
    res = this->NamespaceMemberDefinition();
    if(res == SRV_SUCCESS) {
        finalRes = SRV_SUCCESS;
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        res = this->OnSuccess_NamespaceMemberDefinition();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        res = this->OnFail_NamespaceMemberDefinition();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SPRV_NOT_THIS_RULE) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        _tprintf(TEXT("%snot this rule.\n"), this->tab);
        return SPRV_NOT_THIS_RULE;
    }
    else if(res == SPRV_EOF) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        this->removeTab();
        return SPRV_EOF;
    }
    else if(res != SPRV_CAN_CONTINUE) {
        this->removeTab();
        return res;
    }

    this->removeTab();
    return finalRes;
}

// NamespaceMemberDefinition :: = NamespaceDefinition | TypeDefinition ; 
scriptRetCode scriptParser::NamespaceMemberDefinition() {

    scriptRetCode res;

    this->addTab();
    _tprintf(TEXT("%schecking NamespaceMemberDefinition_part0\n"), this->tab);
    res = this->NamespaceMemberDefinition_part0();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_NamespaceMemberDefinition_part0();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_NamespaceMemberDefinition_part0();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
    }
    _tprintf(TEXT("%schecking NamespaceMemberDefinition_part1\n"), this->tab);
    res = this->NamespaceMemberDefinition_part1();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_NamespaceMemberDefinition_part1();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_NamespaceMemberDefinition_part1();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
    }
    _tprintf(TEXT("%snot this rule.\n"), this->tab);
    this->removeTab();
    return SPRV_NOT_THIS_RULE;
}

// NamespaceDefinition 
scriptRetCode scriptParser::NamespaceMemberDefinition_part0() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
    _tprintf(TEXT("%schecking NamespaceDefinition\n"), this->tab);
    res = this->NamespaceDefinition();
    if(res == SRV_SUCCESS) {
        finalRes = SRV_SUCCESS;
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        res = this->OnSuccess_NamespaceDefinition();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        res = this->OnFail_NamespaceDefinition();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SPRV_NOT_THIS_RULE) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        _tprintf(TEXT("%snot this rule.\n"), this->tab);
        return SPRV_NOT_THIS_RULE;
    }
    else if(res == SPRV_EOF) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        this->removeTab();
        return SPRV_EOF;
    }
    else if(res != SPRV_CAN_CONTINUE) {
        this->removeTab();
        return res;
    }

    this->removeTab();
    return finalRes;
}

// TypeDefinition 
scriptRetCode scriptParser::NamespaceMemberDefinition_part1() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
    _tprintf(TEXT("%schecking TypeDefinition\n"), this->tab);
    res = this->TypeDefinition();
    if(res == SRV_SUCCESS) {
        finalRes = SRV_SUCCESS;
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        res = this->OnSuccess_TypeDefinition();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        res = this->OnFail_TypeDefinition();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SPRV_NOT_THIS_RULE) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        _tprintf(TEXT("%snot this rule.\n"), this->tab);
        return SPRV_NOT_THIS_RULE;
    }
    else if(res == SPRV_EOF) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        this->removeTab();
        return SPRV_EOF;
    }
    else if(res != SPRV_CAN_CONTINUE) {
        this->removeTab();
        return res;
    }

    this->removeTab();
    return finalRes;
}

// TypeDefinition :: = TypeModifier ( EnumDefinition | ClassDefinition | InterfaceDefinition ) ; 
scriptRetCode scriptParser::TypeDefinition() {

    scriptRetCode res;

    this->addTab();
    _tprintf(TEXT("%schecking TypeDefinition_part0\n"), this->tab);
    res = this->TypeDefinition_part0();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_TypeDefinition_part0();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_TypeDefinition_part0();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
    }
    _tprintf(TEXT("%snot this rule.\n"), this->tab);
    this->removeTab();
    return SPRV_NOT_THIS_RULE;
}

// TypeModifier ( EnumDefinition | ClassDefinition | InterfaceDefinition ) 
scriptRetCode scriptParser::TypeDefinition_part0() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
    _tprintf(TEXT("%schecking TypeModifier\n"), this->tab);
    res = this->TypeModifier();
    if(res == SRV_SUCCESS) {
        finalRes = SRV_SUCCESS;
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        res = this->OnSuccess_TypeModifier();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        res = this->OnFail_TypeModifier();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SPRV_NOT_THIS_RULE) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        _tprintf(TEXT("%snot this rule.\n"), this->tab);
        return SPRV_NOT_THIS_RULE;
    }
    else if(res == SPRV_EOF) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        this->removeTab();
        return SPRV_EOF;
    }
    else if(res != SPRV_CAN_CONTINUE) {
        this->removeTab();
        return res;
    }
    _tprintf(TEXT("%schecking TypeDefinition_part0_inplace0\n"), this->tab);
    res = this->TypeDefinition_part0_inplace0();
    if(res == SRV_SUCCESS) {
        finalRes = SRV_SUCCESS;
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        res = this->OnSuccess_TypeDefinition_part0_inplace0();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        res = this->OnFail_TypeDefinition_part0_inplace0();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SPRV_NOT_THIS_RULE) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        _tprintf(TEXT("%snot this rule.\n"), this->tab);
        return SPRV_NOT_THIS_RULE;
    }
    else if(res == SPRV_EOF) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        this->removeTab();
        return SPRV_EOF;
    }
    else if(res != SPRV_CAN_CONTINUE) {
        this->removeTab();
        return res;
    }

    this->removeTab();
    return finalRes;
}

// ( EnumDefinition | ClassDefinition | InterfaceDefinition ) 
scriptRetCode scriptParser::TypeDefinition_part0_inplace0() {

    scriptRetCode res;

    this->addTab();
    _tprintf(TEXT("%schecking TypeDefinition_part0_inplace0_part0\n"), this->tab);
    res = this->TypeDefinition_part0_inplace0_part0();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_TypeDefinition_part0_inplace0_part0();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_TypeDefinition_part0_inplace0_part0();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
    }
    _tprintf(TEXT("%schecking TypeDefinition_part0_inplace0_part1\n"), this->tab);
    res = this->TypeDefinition_part0_inplace0_part1();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_TypeDefinition_part0_inplace0_part1();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_TypeDefinition_part0_inplace0_part1();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
    }
    _tprintf(TEXT("%schecking TypeDefinition_part0_inplace0_part2\n"), this->tab);
    res = this->TypeDefinition_part0_inplace0_part2();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_TypeDefinition_part0_inplace0_part2();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_TypeDefinition_part0_inplace0_part2();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
    }
    _tprintf(TEXT("%snot this rule.\n"), this->tab);
    this->removeTab();
    return SPRV_NOT_THIS_RULE;
}

// EnumDefinition 
scriptRetCode scriptParser::TypeDefinition_part0_inplace0_part0() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
    _tprintf(TEXT("%schecking EnumDefinition\n"), this->tab);
    res = this->EnumDefinition();
    if(res == SRV_SUCCESS) {
        finalRes = SRV_SUCCESS;
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        res = this->OnSuccess_EnumDefinition();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        res = this->OnFail_EnumDefinition();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SPRV_NOT_THIS_RULE) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        _tprintf(TEXT("%snot this rule.\n"), this->tab);
        return SPRV_NOT_THIS_RULE;
    }
    else if(res == SPRV_EOF) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        this->removeTab();
        return SPRV_EOF;
    }
    else if(res != SPRV_CAN_CONTINUE) {
        this->removeTab();
        return res;
    }

    this->removeTab();
    return finalRes;
}

// ClassDefinition 
scriptRetCode scriptParser::TypeDefinition_part0_inplace0_part1() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
    _tprintf(TEXT("%schecking ClassDefinition\n"), this->tab);
    res = this->ClassDefinition();
    if(res == SRV_SUCCESS) {
        finalRes = SRV_SUCCESS;
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        res = this->OnSuccess_ClassDefinition();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        res = this->OnFail_ClassDefinition();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SPRV_NOT_THIS_RULE) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        _tprintf(TEXT("%snot this rule.\n"), this->tab);
        return SPRV_NOT_THIS_RULE;
    }
    else if(res == SPRV_EOF) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        this->removeTab();
        return SPRV_EOF;
    }
    else if(res != SPRV_CAN_CONTINUE) {
        this->removeTab();
        return res;
    }

    this->removeTab();
    return finalRes;
}

// InterfaceDefinition 
scriptRetCode scriptParser::TypeDefinition_part0_inplace0_part2() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
    _tprintf(TEXT("%schecking InterfaceDefinition\n"), this->tab);
    res = this->InterfaceDefinition();
    if(res == SRV_SUCCESS) {
        finalRes = SRV_SUCCESS;
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        res = this->OnSuccess_InterfaceDefinition();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        res = this->OnFail_InterfaceDefinition();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SPRV_NOT_THIS_RULE) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        _tprintf(TEXT("%snot this rule.\n"), this->tab);
        return SPRV_NOT_THIS_RULE;
    }
    else if(res == SPRV_EOF) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        this->removeTab();
        return SPRV_EOF;
    }
    else if(res != SPRV_CAN_CONTINUE) {
        this->removeTab();
        return res;
    }

    this->removeTab();
    return finalRes;
}

// EnumDefinition :: = enum __sgrammar_identifier "{" EnumBodyDefinition "}" ; 
scriptRetCode scriptParser::EnumDefinition() {

    scriptRetCode res;

    this->addTab();
    _tprintf(TEXT("%schecking EnumDefinition_part0\n"), this->tab);
    res = this->EnumDefinition_part0();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_EnumDefinition_part0();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_EnumDefinition_part0();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
    }
    _tprintf(TEXT("%snot this rule.\n"), this->tab);
    this->removeTab();
    return SPRV_NOT_THIS_RULE;
}

// enum __sgrammar_identifier "{" EnumBodyDefinition "}" 
scriptRetCode scriptParser::EnumDefinition_part0() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
	res = this->isRword( LRW_ENUM );
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }
	res = this->isId();
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }
    res = this->isParser( LPT_LEFT_FIGURE_BRACKET );
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }
    _tprintf(TEXT("%schecking EnumBodyDefinition\n"), this->tab);
    res = this->EnumBodyDefinition();
    if(res == SRV_SUCCESS) {
        finalRes = SRV_SUCCESS;
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        res = this->OnSuccess_EnumBodyDefinition();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        res = this->OnFail_EnumBodyDefinition();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SPRV_NOT_THIS_RULE) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        _tprintf(TEXT("%snot this rule.\n"), this->tab);
        return SPRV_NOT_THIS_RULE;
    }
    else if(res == SPRV_EOF) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        this->removeTab();
        return SPRV_EOF;
    }
    else if(res != SPRV_CAN_CONTINUE) {
        this->removeTab();
        return res;
    }
    res = this->isParser( LPT_RIGHT_FIGURE_BRACKET );
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }

    this->removeTab();
    return finalRes;
}

// EnumBodyDefinition :: = EnumItemDefinition { "," EnumItemDefinition } ; 
scriptRetCode scriptParser::EnumBodyDefinition() {

    scriptRetCode res;

    this->addTab();
    _tprintf(TEXT("%schecking EnumBodyDefinition_part0\n"), this->tab);
    res = this->EnumBodyDefinition_part0();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_EnumBodyDefinition_part0();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_EnumBodyDefinition_part0();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
    }
    _tprintf(TEXT("%snot this rule.\n"), this->tab);
    this->removeTab();
    return SPRV_NOT_THIS_RULE;
}

// EnumItemDefinition { "," EnumItemDefinition } 
scriptRetCode scriptParser::EnumBodyDefinition_part0() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
    _tprintf(TEXT("%schecking EnumItemDefinition\n"), this->tab);
    res = this->EnumItemDefinition();
    if(res == SRV_SUCCESS) {
        finalRes = SRV_SUCCESS;
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        res = this->OnSuccess_EnumItemDefinition();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        res = this->OnFail_EnumItemDefinition();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SPRV_NOT_THIS_RULE) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        _tprintf(TEXT("%snot this rule.\n"), this->tab);
        return SPRV_NOT_THIS_RULE;
    }
    else if(res == SPRV_EOF) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        this->removeTab();
        return SPRV_EOF;
    }
    else if(res != SPRV_CAN_CONTINUE) {
        this->removeTab();
        return res;
    }
    _tprintf(TEXT("%schecking EnumBodyDefinition_part0_inplace0\n"), this->tab);
    res = this->EnumBodyDefinition_part0_inplace0();
    if(res == SRV_SUCCESS) {
        finalRes = SRV_SUCCESS;
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        res = this->OnSuccess_EnumBodyDefinition_part0_inplace0();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        res = this->OnFail_EnumBodyDefinition_part0_inplace0();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SPRV_NOT_THIS_RULE) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        _tprintf(TEXT("%snot this rule.\n"), this->tab);
        return SPRV_NOT_THIS_RULE;
    }
    else if(res == SPRV_EOF) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        this->removeTab();
        return SPRV_EOF;
    }
    else if(res != SPRV_CAN_CONTINUE) {
        this->removeTab();
        return res;
    }

    this->removeTab();
    return finalRes;
}

// { "," EnumItemDefinition } 
scriptRetCode scriptParser::EnumBodyDefinition_part0_inplace0() {

    scriptRetCode res;

    bool firstPart = true;
    this->addTab();
    while( true ) {
        _tprintf(TEXT("%schecking EnumBodyDefinition_part0_inplace0_part0\n"), this->tab);
        res = this->EnumBodyDefinition_part0_inplace0_part0();
        if(res == SRV_SUCCESS) {
            _tprintf(TEXT("%ssuccess.\n"), this->tab);
            this->OnSuccess_EnumBodyDefinition_part0_inplace0_part0();
            firstPart = false;
            continue;
        }
        else if(res == SRV_FAIL) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return this->OnFail_EnumBodyDefinition_part0_inplace0_part0();
        }
        else if(res != SPRV_NOT_THIS_RULE) {
            _tprintf(TEXT("%sanother res.\n"), this->tab);
            this->removeTab();
            return res;
        }
        break;
    }
    _tprintf(firstPart? TEXT("%scan continue.\n"): TEXT("%ssuccess.\n"), this->tab);
    this->removeTab();
    return firstPart? SPRV_CAN_CONTINUE: SRV_SUCCESS;
}

// "," EnumItemDefinition 
scriptRetCode scriptParser::EnumBodyDefinition_part0_inplace0_part0() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
    res = this->isParser( LPT_COMMA );
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }
    _tprintf(TEXT("%schecking EnumItemDefinition\n"), this->tab);
    res = this->EnumItemDefinition();
    if(res == SRV_SUCCESS) {
        finalRes = SRV_SUCCESS;
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        res = this->OnSuccess_EnumItemDefinition();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        res = this->OnFail_EnumItemDefinition();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SPRV_NOT_THIS_RULE) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        _tprintf(TEXT("%snot this rule.\n"), this->tab);
        return SPRV_NOT_THIS_RULE;
    }
    else if(res == SPRV_EOF) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        this->removeTab();
        return SPRV_EOF;
    }
    else if(res != SPRV_CAN_CONTINUE) {
        this->removeTab();
        return res;
    }

    this->removeTab();
    return finalRes;
}

// EnumItemDefinition :: = __sgrammar_identifier [ "=" __sgrammar_constValue ] ; 
scriptRetCode scriptParser::EnumItemDefinition() {

    scriptRetCode res;

    this->addTab();
    _tprintf(TEXT("%schecking EnumItemDefinition_part0\n"), this->tab);
    res = this->EnumItemDefinition_part0();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_EnumItemDefinition_part0();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_EnumItemDefinition_part0();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
    }
    _tprintf(TEXT("%snot this rule.\n"), this->tab);
    this->removeTab();
    return SPRV_NOT_THIS_RULE;
}

// __sgrammar_identifier [ "=" __sgrammar_constValue ] 
scriptRetCode scriptParser::EnumItemDefinition_part0() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
	res = this->isId();
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }
    _tprintf(TEXT("%schecking EnumItemDefinition_part0_inplace0\n"), this->tab);
    res = this->EnumItemDefinition_part0_inplace0();
    if(res == SRV_SUCCESS) {
        finalRes = SRV_SUCCESS;
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        res = this->OnSuccess_EnumItemDefinition_part0_inplace0();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        res = this->OnFail_EnumItemDefinition_part0_inplace0();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SPRV_NOT_THIS_RULE) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        _tprintf(TEXT("%snot this rule.\n"), this->tab);
        return SPRV_NOT_THIS_RULE;
    }
    else if(res == SPRV_EOF) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        this->removeTab();
        return SPRV_EOF;
    }
    else if(res != SPRV_CAN_CONTINUE) {
        this->removeTab();
        return res;
    }

    this->removeTab();
    return finalRes;
}

// [ "=" __sgrammar_constValue ] 
scriptRetCode scriptParser::EnumItemDefinition_part0_inplace0() {

    scriptRetCode res;

    this->addTab();
    _tprintf(TEXT("%schecking EnumItemDefinition_part0_inplace0_part0\n"), this->tab);
    res = this->EnumItemDefinition_part0_inplace0_part0();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_EnumItemDefinition_part0_inplace0_part0();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_EnumItemDefinition_part0_inplace0_part0();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
        return res;
    }
    _tprintf(TEXT("%scan continue.\n"), this->tab);
    this->removeTab();
    return SPRV_CAN_CONTINUE;
}

// "=" __sgrammar_constValue 
scriptRetCode scriptParser::EnumItemDefinition_part0_inplace0_part0() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
    res = this->isParser( LPT_ASSIGNMENT );
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }
	res = this->isConstValue();
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }

    this->removeTab();
    return finalRes;
}

// ClassDefinition :: = class __sgrammar_identifier "{" "}" ; 
scriptRetCode scriptParser::ClassDefinition() {

    scriptRetCode res;

    this->addTab();
    _tprintf(TEXT("%schecking ClassDefinition_part0\n"), this->tab);
    res = this->ClassDefinition_part0();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_ClassDefinition_part0();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_ClassDefinition_part0();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
    }
    _tprintf(TEXT("%snot this rule.\n"), this->tab);
    this->removeTab();
    return SPRV_NOT_THIS_RULE;
}

// class __sgrammar_identifier "{" "}" 
scriptRetCode scriptParser::ClassDefinition_part0() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
	res = this->isRword( LRW_CLASS );
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }
	res = this->isId();
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }
    res = this->isParser( LPT_LEFT_FIGURE_BRACKET );
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }
    res = this->isParser( LPT_RIGHT_FIGURE_BRACKET );
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }

    this->removeTab();
    return finalRes;
}

// InterfaceDefinition :: = interface __sgrammar_identifier [ DirevedFromList ] "{" InterfaceBody "}" ; 
scriptRetCode scriptParser::InterfaceDefinition() {

    scriptRetCode res;

    this->addTab();
    _tprintf(TEXT("%schecking InterfaceDefinition_part0\n"), this->tab);
    res = this->InterfaceDefinition_part0();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_InterfaceDefinition_part0();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_InterfaceDefinition_part0();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
    }
    _tprintf(TEXT("%snot this rule.\n"), this->tab);
    this->removeTab();
    return SPRV_NOT_THIS_RULE;
}

// interface __sgrammar_identifier [ DirevedFromList ] "{" InterfaceBody "}" 
scriptRetCode scriptParser::InterfaceDefinition_part0() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
	res = this->isRword( LRW_INTERFACE );
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }
	res = this->isId();
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }
    _tprintf(TEXT("%schecking InterfaceDefinition_part0_inplace0\n"), this->tab);
    res = this->InterfaceDefinition_part0_inplace0();
    if(res == SRV_SUCCESS) {
        finalRes = SRV_SUCCESS;
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        res = this->OnSuccess_InterfaceDefinition_part0_inplace0();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        res = this->OnFail_InterfaceDefinition_part0_inplace0();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SPRV_NOT_THIS_RULE) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        _tprintf(TEXT("%snot this rule.\n"), this->tab);
        return SPRV_NOT_THIS_RULE;
    }
    else if(res == SPRV_EOF) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        this->removeTab();
        return SPRV_EOF;
    }
    else if(res != SPRV_CAN_CONTINUE) {
        this->removeTab();
        return res;
    }
    res = this->isParser( LPT_LEFT_FIGURE_BRACKET );
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }
    _tprintf(TEXT("%schecking InterfaceBody\n"), this->tab);
    res = this->InterfaceBody();
    if(res == SRV_SUCCESS) {
        finalRes = SRV_SUCCESS;
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        res = this->OnSuccess_InterfaceBody();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        res = this->OnFail_InterfaceBody();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SPRV_NOT_THIS_RULE) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        _tprintf(TEXT("%snot this rule.\n"), this->tab);
        return SPRV_NOT_THIS_RULE;
    }
    else if(res == SPRV_EOF) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        this->removeTab();
        return SPRV_EOF;
    }
    else if(res != SPRV_CAN_CONTINUE) {
        this->removeTab();
        return res;
    }
    res = this->isParser( LPT_RIGHT_FIGURE_BRACKET );
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }

    this->removeTab();
    return finalRes;
}

// [ DirevedFromList ] 
scriptRetCode scriptParser::InterfaceDefinition_part0_inplace0() {

    scriptRetCode res;

    this->addTab();
    _tprintf(TEXT("%schecking InterfaceDefinition_part0_inplace0_part0\n"), this->tab);
    res = this->InterfaceDefinition_part0_inplace0_part0();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_InterfaceDefinition_part0_inplace0_part0();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_InterfaceDefinition_part0_inplace0_part0();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
        return res;
    }
    _tprintf(TEXT("%scan continue.\n"), this->tab);
    this->removeTab();
    return SPRV_CAN_CONTINUE;
}

// DirevedFromList 
scriptRetCode scriptParser::InterfaceDefinition_part0_inplace0_part0() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
    _tprintf(TEXT("%schecking DirevedFromList\n"), this->tab);
    res = this->DirevedFromList();
    if(res == SRV_SUCCESS) {
        finalRes = SRV_SUCCESS;
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        res = this->OnSuccess_DirevedFromList();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        res = this->OnFail_DirevedFromList();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SPRV_NOT_THIS_RULE) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        _tprintf(TEXT("%snot this rule.\n"), this->tab);
        return SPRV_NOT_THIS_RULE;
    }
    else if(res == SPRV_EOF) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        this->removeTab();
        return SPRV_EOF;
    }
    else if(res != SPRV_CAN_CONTINUE) {
        this->removeTab();
        return res;
    }

    this->removeTab();
    return finalRes;
}

// DirevedFromList :: = ":" QualifiedTypeName { "," QualifiedTypeName } ; 
scriptRetCode scriptParser::DirevedFromList() {

    scriptRetCode res;

    this->addTab();
    _tprintf(TEXT("%schecking DirevedFromList_part0\n"), this->tab);
    res = this->DirevedFromList_part0();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_DirevedFromList_part0();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_DirevedFromList_part0();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
    }
    _tprintf(TEXT("%snot this rule.\n"), this->tab);
    this->removeTab();
    return SPRV_NOT_THIS_RULE;
}

// ":" QualifiedTypeName { "," QualifiedTypeName } 
scriptRetCode scriptParser::DirevedFromList_part0() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
    res = this->isParser( LPT_DOT2 );
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }
    _tprintf(TEXT("%schecking QualifiedTypeName\n"), this->tab);
    res = this->QualifiedTypeName();
    if(res == SRV_SUCCESS) {
        finalRes = SRV_SUCCESS;
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        res = this->OnSuccess_QualifiedTypeName();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        res = this->OnFail_QualifiedTypeName();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SPRV_NOT_THIS_RULE) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        _tprintf(TEXT("%snot this rule.\n"), this->tab);
        return SPRV_NOT_THIS_RULE;
    }
    else if(res == SPRV_EOF) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        this->removeTab();
        return SPRV_EOF;
    }
    else if(res != SPRV_CAN_CONTINUE) {
        this->removeTab();
        return res;
    }
    _tprintf(TEXT("%schecking DirevedFromList_part0_inplace0\n"), this->tab);
    res = this->DirevedFromList_part0_inplace0();
    if(res == SRV_SUCCESS) {
        finalRes = SRV_SUCCESS;
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        res = this->OnSuccess_DirevedFromList_part0_inplace0();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        res = this->OnFail_DirevedFromList_part0_inplace0();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SPRV_NOT_THIS_RULE) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        _tprintf(TEXT("%snot this rule.\n"), this->tab);
        return SPRV_NOT_THIS_RULE;
    }
    else if(res == SPRV_EOF) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        this->removeTab();
        return SPRV_EOF;
    }
    else if(res != SPRV_CAN_CONTINUE) {
        this->removeTab();
        return res;
    }

    this->removeTab();
    return finalRes;
}

// { "," QualifiedTypeName } 
scriptRetCode scriptParser::DirevedFromList_part0_inplace0() {

    scriptRetCode res;

    bool firstPart = true;
    this->addTab();
    while( true ) {
        _tprintf(TEXT("%schecking DirevedFromList_part0_inplace0_part0\n"), this->tab);
        res = this->DirevedFromList_part0_inplace0_part0();
        if(res == SRV_SUCCESS) {
            _tprintf(TEXT("%ssuccess.\n"), this->tab);
            this->OnSuccess_DirevedFromList_part0_inplace0_part0();
            firstPart = false;
            continue;
        }
        else if(res == SRV_FAIL) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return this->OnFail_DirevedFromList_part0_inplace0_part0();
        }
        else if(res != SPRV_NOT_THIS_RULE) {
            _tprintf(TEXT("%sanother res.\n"), this->tab);
            this->removeTab();
            return res;
        }
        break;
    }
    _tprintf(firstPart? TEXT("%scan continue.\n"): TEXT("%ssuccess.\n"), this->tab);
    this->removeTab();
    return firstPart? SPRV_CAN_CONTINUE: SRV_SUCCESS;
}

// "," QualifiedTypeName 
scriptRetCode scriptParser::DirevedFromList_part0_inplace0_part0() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
    res = this->isParser( LPT_COMMA );
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }
    _tprintf(TEXT("%schecking QualifiedTypeName\n"), this->tab);
    res = this->QualifiedTypeName();
    if(res == SRV_SUCCESS) {
        finalRes = SRV_SUCCESS;
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        res = this->OnSuccess_QualifiedTypeName();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        res = this->OnFail_QualifiedTypeName();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SPRV_NOT_THIS_RULE) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        _tprintf(TEXT("%snot this rule.\n"), this->tab);
        return SPRV_NOT_THIS_RULE;
    }
    else if(res == SPRV_EOF) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        this->removeTab();
        return SPRV_EOF;
    }
    else if(res != SPRV_CAN_CONTINUE) {
        this->removeTab();
        return res;
    }

    this->removeTab();
    return finalRes;
}

// InterfaceBody :: = { MethodOrPropertyDeclaration ( MethodDeclaration | PropertyDeclaration ) } ; 
scriptRetCode scriptParser::InterfaceBody() {

    scriptRetCode res;

    this->addTab();
    _tprintf(TEXT("%schecking InterfaceBody_part0\n"), this->tab);
    res = this->InterfaceBody_part0();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_InterfaceBody_part0();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_InterfaceBody_part0();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
    }
    _tprintf(TEXT("%snot this rule.\n"), this->tab);
    this->removeTab();
    return SPRV_NOT_THIS_RULE;
}

// { MethodOrPropertyDeclaration ( MethodDeclaration | PropertyDeclaration ) } 
scriptRetCode scriptParser::InterfaceBody_part0() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
    _tprintf(TEXT("%schecking InterfaceBody_part0_inplace0\n"), this->tab);
    res = this->InterfaceBody_part0_inplace0();
    if(res == SRV_SUCCESS) {
        finalRes = SRV_SUCCESS;
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        res = this->OnSuccess_InterfaceBody_part0_inplace0();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        res = this->OnFail_InterfaceBody_part0_inplace0();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SPRV_NOT_THIS_RULE) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        _tprintf(TEXT("%snot this rule.\n"), this->tab);
        return SPRV_NOT_THIS_RULE;
    }
    else if(res == SPRV_EOF) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        this->removeTab();
        return SPRV_EOF;
    }
    else if(res != SPRV_CAN_CONTINUE) {
        this->removeTab();
        return res;
    }

    this->removeTab();
    return finalRes;
}

// { MethodOrPropertyDeclaration ( MethodDeclaration | PropertyDeclaration ) } 
scriptRetCode scriptParser::InterfaceBody_part0_inplace0() {

    scriptRetCode res;

    bool firstPart = true;
    this->addTab();
    while( true ) {
        _tprintf(TEXT("%schecking InterfaceBody_part0_inplace0_part0\n"), this->tab);
        res = this->InterfaceBody_part0_inplace0_part0();
        if(res == SRV_SUCCESS) {
            _tprintf(TEXT("%ssuccess.\n"), this->tab);
            this->OnSuccess_InterfaceBody_part0_inplace0_part0();
            firstPart = false;
            continue;
        }
        else if(res == SRV_FAIL) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return this->OnFail_InterfaceBody_part0_inplace0_part0();
        }
        else if(res != SPRV_NOT_THIS_RULE) {
            _tprintf(TEXT("%sanother res.\n"), this->tab);
            this->removeTab();
            return res;
        }
        break;
    }
    _tprintf(firstPart? TEXT("%scan continue.\n"): TEXT("%ssuccess.\n"), this->tab);
    this->removeTab();
    return firstPart? SPRV_CAN_CONTINUE: SRV_SUCCESS;
}

// MethodOrPropertyDeclaration ( MethodDeclaration | PropertyDeclaration ) 
scriptRetCode scriptParser::InterfaceBody_part0_inplace0_part0() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
    _tprintf(TEXT("%schecking MethodOrPropertyDeclaration\n"), this->tab);
    res = this->MethodOrPropertyDeclaration();
    if(res == SRV_SUCCESS) {
        finalRes = SRV_SUCCESS;
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        res = this->OnSuccess_MethodOrPropertyDeclaration();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        res = this->OnFail_MethodOrPropertyDeclaration();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SPRV_NOT_THIS_RULE) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        _tprintf(TEXT("%snot this rule.\n"), this->tab);
        return SPRV_NOT_THIS_RULE;
    }
    else if(res == SPRV_EOF) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        this->removeTab();
        return SPRV_EOF;
    }
    else if(res != SPRV_CAN_CONTINUE) {
        this->removeTab();
        return res;
    }
    _tprintf(TEXT("%schecking InterfaceBody_part0_inplace0_part0_inplace0\n"), this->tab);
    res = this->InterfaceBody_part0_inplace0_part0_inplace0();
    if(res == SRV_SUCCESS) {
        finalRes = SRV_SUCCESS;
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        res = this->OnSuccess_InterfaceBody_part0_inplace0_part0_inplace0();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        res = this->OnFail_InterfaceBody_part0_inplace0_part0_inplace0();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SPRV_NOT_THIS_RULE) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        _tprintf(TEXT("%snot this rule.\n"), this->tab);
        return SPRV_NOT_THIS_RULE;
    }
    else if(res == SPRV_EOF) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        this->removeTab();
        return SPRV_EOF;
    }
    else if(res != SPRV_CAN_CONTINUE) {
        this->removeTab();
        return res;
    }

    this->removeTab();
    return finalRes;
}

// ( MethodDeclaration | PropertyDeclaration ) 
scriptRetCode scriptParser::InterfaceBody_part0_inplace0_part0_inplace0() {

    scriptRetCode res;

    this->addTab();
    _tprintf(TEXT("%schecking InterfaceBody_part0_inplace0_part0_inplace0_part0\n"), this->tab);
    res = this->InterfaceBody_part0_inplace0_part0_inplace0_part0();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_InterfaceBody_part0_inplace0_part0_inplace0_part0();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_InterfaceBody_part0_inplace0_part0_inplace0_part0();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
    }
    _tprintf(TEXT("%schecking InterfaceBody_part0_inplace0_part0_inplace0_part1\n"), this->tab);
    res = this->InterfaceBody_part0_inplace0_part0_inplace0_part1();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_InterfaceBody_part0_inplace0_part0_inplace0_part1();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_InterfaceBody_part0_inplace0_part0_inplace0_part1();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
    }
    _tprintf(TEXT("%snot this rule.\n"), this->tab);
    this->removeTab();
    return SPRV_NOT_THIS_RULE;
}

// MethodDeclaration 
scriptRetCode scriptParser::InterfaceBody_part0_inplace0_part0_inplace0_part0() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
    _tprintf(TEXT("%schecking MethodDeclaration\n"), this->tab);
    res = this->MethodDeclaration();
    if(res == SRV_SUCCESS) {
        finalRes = SRV_SUCCESS;
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        res = this->OnSuccess_MethodDeclaration();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        res = this->OnFail_MethodDeclaration();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SPRV_NOT_THIS_RULE) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        _tprintf(TEXT("%snot this rule.\n"), this->tab);
        return SPRV_NOT_THIS_RULE;
    }
    else if(res == SPRV_EOF) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        this->removeTab();
        return SPRV_EOF;
    }
    else if(res != SPRV_CAN_CONTINUE) {
        this->removeTab();
        return res;
    }

    this->removeTab();
    return finalRes;
}

// PropertyDeclaration 
scriptRetCode scriptParser::InterfaceBody_part0_inplace0_part0_inplace0_part1() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
    _tprintf(TEXT("%schecking PropertyDeclaration\n"), this->tab);
    res = this->PropertyDeclaration();
    if(res == SRV_SUCCESS) {
        finalRes = SRV_SUCCESS;
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        res = this->OnSuccess_PropertyDeclaration();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        res = this->OnFail_PropertyDeclaration();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SPRV_NOT_THIS_RULE) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        _tprintf(TEXT("%snot this rule.\n"), this->tab);
        return SPRV_NOT_THIS_RULE;
    }
    else if(res == SPRV_EOF) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        this->removeTab();
        return SPRV_EOF;
    }
    else if(res != SPRV_CAN_CONTINUE) {
        this->removeTab();
        return res;
    }

    this->removeTab();
    return finalRes;
}

// MethodOrPropertyDeclaration :: = ReturnTypeDefinition MethodOrPropertyName ; 
scriptRetCode scriptParser::MethodOrPropertyDeclaration() {

    scriptRetCode res;

    this->addTab();
    _tprintf(TEXT("%schecking MethodOrPropertyDeclaration_part0\n"), this->tab);
    res = this->MethodOrPropertyDeclaration_part0();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_MethodOrPropertyDeclaration_part0();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_MethodOrPropertyDeclaration_part0();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
    }
    _tprintf(TEXT("%snot this rule.\n"), this->tab);
    this->removeTab();
    return SPRV_NOT_THIS_RULE;
}

// ReturnTypeDefinition MethodOrPropertyName 
scriptRetCode scriptParser::MethodOrPropertyDeclaration_part0() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
    _tprintf(TEXT("%schecking ReturnTypeDefinition\n"), this->tab);
    res = this->ReturnTypeDefinition();
    if(res == SRV_SUCCESS) {
        finalRes = SRV_SUCCESS;
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        res = this->OnSuccess_ReturnTypeDefinition();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        res = this->OnFail_ReturnTypeDefinition();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SPRV_NOT_THIS_RULE) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        _tprintf(TEXT("%snot this rule.\n"), this->tab);
        return SPRV_NOT_THIS_RULE;
    }
    else if(res == SPRV_EOF) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        this->removeTab();
        return SPRV_EOF;
    }
    else if(res != SPRV_CAN_CONTINUE) {
        this->removeTab();
        return res;
    }
    _tprintf(TEXT("%schecking MethodOrPropertyName\n"), this->tab);
    res = this->MethodOrPropertyName();
    if(res == SRV_SUCCESS) {
        finalRes = SRV_SUCCESS;
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        res = this->OnSuccess_MethodOrPropertyName();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        res = this->OnFail_MethodOrPropertyName();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SPRV_NOT_THIS_RULE) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        _tprintf(TEXT("%snot this rule.\n"), this->tab);
        return SPRV_NOT_THIS_RULE;
    }
    else if(res == SPRV_EOF) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        this->removeTab();
        return SPRV_EOF;
    }
    else if(res != SPRV_CAN_CONTINUE) {
        this->removeTab();
        return res;
    }

    this->removeTab();
    return finalRes;
}

// MethodDeclaration :: = "(" [ ParametersListDeclaration ] ")" ";" ; 
scriptRetCode scriptParser::MethodDeclaration() {

    scriptRetCode res;

    this->addTab();
    _tprintf(TEXT("%schecking MethodDeclaration_part0\n"), this->tab);
    res = this->MethodDeclaration_part0();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_MethodDeclaration_part0();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_MethodDeclaration_part0();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
    }
    _tprintf(TEXT("%snot this rule.\n"), this->tab);
    this->removeTab();
    return SPRV_NOT_THIS_RULE;
}

// "(" [ ParametersListDeclaration ] ")" ";" 
scriptRetCode scriptParser::MethodDeclaration_part0() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
    res = this->isParser( LPT_LEFT_BRACKET );
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }
    _tprintf(TEXT("%schecking MethodDeclaration_part0_inplace0\n"), this->tab);
    res = this->MethodDeclaration_part0_inplace0();
    if(res == SRV_SUCCESS) {
        finalRes = SRV_SUCCESS;
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        res = this->OnSuccess_MethodDeclaration_part0_inplace0();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        res = this->OnFail_MethodDeclaration_part0_inplace0();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SPRV_NOT_THIS_RULE) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        _tprintf(TEXT("%snot this rule.\n"), this->tab);
        return SPRV_NOT_THIS_RULE;
    }
    else if(res == SPRV_EOF) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        this->removeTab();
        return SPRV_EOF;
    }
    else if(res != SPRV_CAN_CONTINUE) {
        this->removeTab();
        return res;
    }
    res = this->isParser( LPT_RIGHT_BRACKET );
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }
    res = this->isParser( LPT_DOTCOMMA );
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }

    this->removeTab();
    return finalRes;
}

// [ ParametersListDeclaration ] 
scriptRetCode scriptParser::MethodDeclaration_part0_inplace0() {

    scriptRetCode res;

    this->addTab();
    _tprintf(TEXT("%schecking MethodDeclaration_part0_inplace0_part0\n"), this->tab);
    res = this->MethodDeclaration_part0_inplace0_part0();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_MethodDeclaration_part0_inplace0_part0();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_MethodDeclaration_part0_inplace0_part0();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
        return res;
    }
    _tprintf(TEXT("%scan continue.\n"), this->tab);
    this->removeTab();
    return SPRV_CAN_CONTINUE;
}

// ParametersListDeclaration 
scriptRetCode scriptParser::MethodDeclaration_part0_inplace0_part0() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
    _tprintf(TEXT("%schecking ParametersListDeclaration\n"), this->tab);
    res = this->ParametersListDeclaration();
    if(res == SRV_SUCCESS) {
        finalRes = SRV_SUCCESS;
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        res = this->OnSuccess_ParametersListDeclaration();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        res = this->OnFail_ParametersListDeclaration();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SPRV_NOT_THIS_RULE) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        _tprintf(TEXT("%snot this rule.\n"), this->tab);
        return SPRV_NOT_THIS_RULE;
    }
    else if(res == SPRV_EOF) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        this->removeTab();
        return SPRV_EOF;
    }
    else if(res != SPRV_CAN_CONTINUE) {
        this->removeTab();
        return res;
    }

    this->removeTab();
    return finalRes;
}

// PropertyDeclaration :: = "{" ( GetThenSetInterfacePropDecl | SetThemGetInterfacePropDecl ) "}" ; 
scriptRetCode scriptParser::PropertyDeclaration() {

    scriptRetCode res;

    this->addTab();
    _tprintf(TEXT("%schecking PropertyDeclaration_part0\n"), this->tab);
    res = this->PropertyDeclaration_part0();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_PropertyDeclaration_part0();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_PropertyDeclaration_part0();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
    }
    _tprintf(TEXT("%snot this rule.\n"), this->tab);
    this->removeTab();
    return SPRV_NOT_THIS_RULE;
}

// "{" ( GetThenSetInterfacePropDecl | SetThemGetInterfacePropDecl ) "}" 
scriptRetCode scriptParser::PropertyDeclaration_part0() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
    res = this->isParser( LPT_LEFT_FIGURE_BRACKET );
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }
    _tprintf(TEXT("%schecking PropertyDeclaration_part0_inplace0\n"), this->tab);
    res = this->PropertyDeclaration_part0_inplace0();
    if(res == SRV_SUCCESS) {
        finalRes = SRV_SUCCESS;
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        res = this->OnSuccess_PropertyDeclaration_part0_inplace0();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        res = this->OnFail_PropertyDeclaration_part0_inplace0();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SPRV_NOT_THIS_RULE) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        _tprintf(TEXT("%snot this rule.\n"), this->tab);
        return SPRV_NOT_THIS_RULE;
    }
    else if(res == SPRV_EOF) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        this->removeTab();
        return SPRV_EOF;
    }
    else if(res != SPRV_CAN_CONTINUE) {
        this->removeTab();
        return res;
    }
    res = this->isParser( LPT_RIGHT_FIGURE_BRACKET );
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }

    this->removeTab();
    return finalRes;
}

// ( GetThenSetInterfacePropDecl | SetThemGetInterfacePropDecl ) 
scriptRetCode scriptParser::PropertyDeclaration_part0_inplace0() {

    scriptRetCode res;

    this->addTab();
    _tprintf(TEXT("%schecking PropertyDeclaration_part0_inplace0_part0\n"), this->tab);
    res = this->PropertyDeclaration_part0_inplace0_part0();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_PropertyDeclaration_part0_inplace0_part0();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_PropertyDeclaration_part0_inplace0_part0();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
    }
    _tprintf(TEXT("%schecking PropertyDeclaration_part0_inplace0_part1\n"), this->tab);
    res = this->PropertyDeclaration_part0_inplace0_part1();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_PropertyDeclaration_part0_inplace0_part1();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_PropertyDeclaration_part0_inplace0_part1();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
    }
    _tprintf(TEXT("%snot this rule.\n"), this->tab);
    this->removeTab();
    return SPRV_NOT_THIS_RULE;
}

// GetThenSetInterfacePropDecl 
scriptRetCode scriptParser::PropertyDeclaration_part0_inplace0_part0() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
    _tprintf(TEXT("%schecking GetThenSetInterfacePropDecl\n"), this->tab);
    res = this->GetThenSetInterfacePropDecl();
    if(res == SRV_SUCCESS) {
        finalRes = SRV_SUCCESS;
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        res = this->OnSuccess_GetThenSetInterfacePropDecl();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        res = this->OnFail_GetThenSetInterfacePropDecl();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SPRV_NOT_THIS_RULE) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        _tprintf(TEXT("%snot this rule.\n"), this->tab);
        return SPRV_NOT_THIS_RULE;
    }
    else if(res == SPRV_EOF) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        this->removeTab();
        return SPRV_EOF;
    }
    else if(res != SPRV_CAN_CONTINUE) {
        this->removeTab();
        return res;
    }

    this->removeTab();
    return finalRes;
}

// SetThemGetInterfacePropDecl 
scriptRetCode scriptParser::PropertyDeclaration_part0_inplace0_part1() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
    _tprintf(TEXT("%schecking SetThemGetInterfacePropDecl\n"), this->tab);
    res = this->SetThemGetInterfacePropDecl();
    if(res == SRV_SUCCESS) {
        finalRes = SRV_SUCCESS;
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        res = this->OnSuccess_SetThemGetInterfacePropDecl();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        res = this->OnFail_SetThemGetInterfacePropDecl();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SPRV_NOT_THIS_RULE) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        _tprintf(TEXT("%snot this rule.\n"), this->tab);
        return SPRV_NOT_THIS_RULE;
    }
    else if(res == SPRV_EOF) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        this->removeTab();
        return SPRV_EOF;
    }
    else if(res != SPRV_CAN_CONTINUE) {
        this->removeTab();
        return res;
    }

    this->removeTab();
    return finalRes;
}

// GetThenSetInterfacePropDecl :: = get ";" [ set ";" ] ; 
scriptRetCode scriptParser::GetThenSetInterfacePropDecl() {

    scriptRetCode res;

    this->addTab();
    _tprintf(TEXT("%schecking GetThenSetInterfacePropDecl_part0\n"), this->tab);
    res = this->GetThenSetInterfacePropDecl_part0();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_GetThenSetInterfacePropDecl_part0();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_GetThenSetInterfacePropDecl_part0();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
    }
    _tprintf(TEXT("%snot this rule.\n"), this->tab);
    this->removeTab();
    return SPRV_NOT_THIS_RULE;
}

// get ";" [ set ";" ] 
scriptRetCode scriptParser::GetThenSetInterfacePropDecl_part0() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
	res = this->isRword( LRW_GET );
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }
    res = this->isParser( LPT_DOTCOMMA );
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }
    _tprintf(TEXT("%schecking GetThenSetInterfacePropDecl_part0_inplace0\n"), this->tab);
    res = this->GetThenSetInterfacePropDecl_part0_inplace0();
    if(res == SRV_SUCCESS) {
        finalRes = SRV_SUCCESS;
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        res = this->OnSuccess_GetThenSetInterfacePropDecl_part0_inplace0();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        res = this->OnFail_GetThenSetInterfacePropDecl_part0_inplace0();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SPRV_NOT_THIS_RULE) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        _tprintf(TEXT("%snot this rule.\n"), this->tab);
        return SPRV_NOT_THIS_RULE;
    }
    else if(res == SPRV_EOF) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        this->removeTab();
        return SPRV_EOF;
    }
    else if(res != SPRV_CAN_CONTINUE) {
        this->removeTab();
        return res;
    }

    this->removeTab();
    return finalRes;
}

// [ set ";" ] 
scriptRetCode scriptParser::GetThenSetInterfacePropDecl_part0_inplace0() {

    scriptRetCode res;

    this->addTab();
    _tprintf(TEXT("%schecking GetThenSetInterfacePropDecl_part0_inplace0_part0\n"), this->tab);
    res = this->GetThenSetInterfacePropDecl_part0_inplace0_part0();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_GetThenSetInterfacePropDecl_part0_inplace0_part0();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_GetThenSetInterfacePropDecl_part0_inplace0_part0();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
        return res;
    }
    _tprintf(TEXT("%scan continue.\n"), this->tab);
    this->removeTab();
    return SPRV_CAN_CONTINUE;
}

// set ";" 
scriptRetCode scriptParser::GetThenSetInterfacePropDecl_part0_inplace0_part0() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
	res = this->isRword( LRW_SET );
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }
    res = this->isParser( LPT_DOTCOMMA );
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }

    this->removeTab();
    return finalRes;
}

// SetThemGetInterfacePropDecl :: = set ";" [ get ";" ] ; 
scriptRetCode scriptParser::SetThemGetInterfacePropDecl() {

    scriptRetCode res;

    this->addTab();
    _tprintf(TEXT("%schecking SetThemGetInterfacePropDecl_part0\n"), this->tab);
    res = this->SetThemGetInterfacePropDecl_part0();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_SetThemGetInterfacePropDecl_part0();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_SetThemGetInterfacePropDecl_part0();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
    }
    _tprintf(TEXT("%snot this rule.\n"), this->tab);
    this->removeTab();
    return SPRV_NOT_THIS_RULE;
}

// set ";" [ get ";" ] 
scriptRetCode scriptParser::SetThemGetInterfacePropDecl_part0() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
	res = this->isRword( LRW_SET );
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }
    res = this->isParser( LPT_DOTCOMMA );
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }
    _tprintf(TEXT("%schecking SetThemGetInterfacePropDecl_part0_inplace0\n"), this->tab);
    res = this->SetThemGetInterfacePropDecl_part0_inplace0();
    if(res == SRV_SUCCESS) {
        finalRes = SRV_SUCCESS;
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        res = this->OnSuccess_SetThemGetInterfacePropDecl_part0_inplace0();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        res = this->OnFail_SetThemGetInterfacePropDecl_part0_inplace0();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SPRV_NOT_THIS_RULE) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        _tprintf(TEXT("%snot this rule.\n"), this->tab);
        return SPRV_NOT_THIS_RULE;
    }
    else if(res == SPRV_EOF) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        this->removeTab();
        return SPRV_EOF;
    }
    else if(res != SPRV_CAN_CONTINUE) {
        this->removeTab();
        return res;
    }

    this->removeTab();
    return finalRes;
}

// [ get ";" ] 
scriptRetCode scriptParser::SetThemGetInterfacePropDecl_part0_inplace0() {

    scriptRetCode res;

    this->addTab();
    _tprintf(TEXT("%schecking SetThemGetInterfacePropDecl_part0_inplace0_part0\n"), this->tab);
    res = this->SetThemGetInterfacePropDecl_part0_inplace0_part0();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_SetThemGetInterfacePropDecl_part0_inplace0_part0();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_SetThemGetInterfacePropDecl_part0_inplace0_part0();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
        return res;
    }
    _tprintf(TEXT("%scan continue.\n"), this->tab);
    this->removeTab();
    return SPRV_CAN_CONTINUE;
}

// get ";" 
scriptRetCode scriptParser::SetThemGetInterfacePropDecl_part0_inplace0_part0() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
	res = this->isRword( LRW_GET );
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }
    res = this->isParser( LPT_DOTCOMMA );
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }

    this->removeTab();
    return finalRes;
}

// MethodOrPropertyName :: = __sgrammar_identifier ; 
scriptRetCode scriptParser::MethodOrPropertyName() {

    scriptRetCode res;

    this->addTab();
    _tprintf(TEXT("%schecking MethodOrPropertyName_part0\n"), this->tab);
    res = this->MethodOrPropertyName_part0();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_MethodOrPropertyName_part0();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_MethodOrPropertyName_part0();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
    }
    _tprintf(TEXT("%snot this rule.\n"), this->tab);
    this->removeTab();
    return SPRV_NOT_THIS_RULE;
}

// __sgrammar_identifier 
scriptRetCode scriptParser::MethodOrPropertyName_part0() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
	res = this->isId();
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }

    this->removeTab();
    return finalRes;
}

// ParametersListDeclaration :: = ReturnTypeDefinition [ __sgrammar_identifier ] { "," ReturnTypeDefinition [ __sgrammar_identifier ] } ; 
scriptRetCode scriptParser::ParametersListDeclaration() {

    scriptRetCode res;

    this->addTab();
    _tprintf(TEXT("%schecking ParametersListDeclaration_part0\n"), this->tab);
    res = this->ParametersListDeclaration_part0();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_ParametersListDeclaration_part0();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_ParametersListDeclaration_part0();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
    }
    _tprintf(TEXT("%snot this rule.\n"), this->tab);
    this->removeTab();
    return SPRV_NOT_THIS_RULE;
}

// ReturnTypeDefinition [ __sgrammar_identifier ] { "," ReturnTypeDefinition [ __sgrammar_identifier ] } 
scriptRetCode scriptParser::ParametersListDeclaration_part0() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
    _tprintf(TEXT("%schecking ReturnTypeDefinition\n"), this->tab);
    res = this->ReturnTypeDefinition();
    if(res == SRV_SUCCESS) {
        finalRes = SRV_SUCCESS;
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        res = this->OnSuccess_ReturnTypeDefinition();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        res = this->OnFail_ReturnTypeDefinition();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SPRV_NOT_THIS_RULE) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        _tprintf(TEXT("%snot this rule.\n"), this->tab);
        return SPRV_NOT_THIS_RULE;
    }
    else if(res == SPRV_EOF) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        this->removeTab();
        return SPRV_EOF;
    }
    else if(res != SPRV_CAN_CONTINUE) {
        this->removeTab();
        return res;
    }
    _tprintf(TEXT("%schecking ParametersListDeclaration_part0_inplace0\n"), this->tab);
    res = this->ParametersListDeclaration_part0_inplace0();
    if(res == SRV_SUCCESS) {
        finalRes = SRV_SUCCESS;
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        res = this->OnSuccess_ParametersListDeclaration_part0_inplace0();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        res = this->OnFail_ParametersListDeclaration_part0_inplace0();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SPRV_NOT_THIS_RULE) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        _tprintf(TEXT("%snot this rule.\n"), this->tab);
        return SPRV_NOT_THIS_RULE;
    }
    else if(res == SPRV_EOF) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        this->removeTab();
        return SPRV_EOF;
    }
    else if(res != SPRV_CAN_CONTINUE) {
        this->removeTab();
        return res;
    }
    _tprintf(TEXT("%schecking ParametersListDeclaration_part0_inplace1\n"), this->tab);
    res = this->ParametersListDeclaration_part0_inplace1();
    if(res == SRV_SUCCESS) {
        finalRes = SRV_SUCCESS;
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        res = this->OnSuccess_ParametersListDeclaration_part0_inplace1();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        res = this->OnFail_ParametersListDeclaration_part0_inplace1();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SPRV_NOT_THIS_RULE) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        _tprintf(TEXT("%snot this rule.\n"), this->tab);
        return SPRV_NOT_THIS_RULE;
    }
    else if(res == SPRV_EOF) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        this->removeTab();
        return SPRV_EOF;
    }
    else if(res != SPRV_CAN_CONTINUE) {
        this->removeTab();
        return res;
    }

    this->removeTab();
    return finalRes;
}

// [ __sgrammar_identifier ] 
scriptRetCode scriptParser::ParametersListDeclaration_part0_inplace0() {

    scriptRetCode res;

    this->addTab();
    _tprintf(TEXT("%schecking ParametersListDeclaration_part0_inplace0_part0\n"), this->tab);
    res = this->ParametersListDeclaration_part0_inplace0_part0();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_ParametersListDeclaration_part0_inplace0_part0();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_ParametersListDeclaration_part0_inplace0_part0();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
        return res;
    }
    _tprintf(TEXT("%scan continue.\n"), this->tab);
    this->removeTab();
    return SPRV_CAN_CONTINUE;
}

// __sgrammar_identifier 
scriptRetCode scriptParser::ParametersListDeclaration_part0_inplace0_part0() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
	res = this->isId();
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }

    this->removeTab();
    return finalRes;
}

// { "," ReturnTypeDefinition [ __sgrammar_identifier ] } 
scriptRetCode scriptParser::ParametersListDeclaration_part0_inplace1() {

    scriptRetCode res;

    bool firstPart = true;
    this->addTab();
    while( true ) {
        _tprintf(TEXT("%schecking ParametersListDeclaration_part0_inplace1_part0\n"), this->tab);
        res = this->ParametersListDeclaration_part0_inplace1_part0();
        if(res == SRV_SUCCESS) {
            _tprintf(TEXT("%ssuccess.\n"), this->tab);
            this->OnSuccess_ParametersListDeclaration_part0_inplace1_part0();
            firstPart = false;
            continue;
        }
        else if(res == SRV_FAIL) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return this->OnFail_ParametersListDeclaration_part0_inplace1_part0();
        }
        else if(res != SPRV_NOT_THIS_RULE) {
            _tprintf(TEXT("%sanother res.\n"), this->tab);
            this->removeTab();
            return res;
        }
        break;
    }
    _tprintf(firstPart? TEXT("%scan continue.\n"): TEXT("%ssuccess.\n"), this->tab);
    this->removeTab();
    return firstPart? SPRV_CAN_CONTINUE: SRV_SUCCESS;
}

// "," ReturnTypeDefinition [ __sgrammar_identifier ] 
scriptRetCode scriptParser::ParametersListDeclaration_part0_inplace1_part0() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
    res = this->isParser( LPT_COMMA );
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }
    _tprintf(TEXT("%schecking ReturnTypeDefinition\n"), this->tab);
    res = this->ReturnTypeDefinition();
    if(res == SRV_SUCCESS) {
        finalRes = SRV_SUCCESS;
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        res = this->OnSuccess_ReturnTypeDefinition();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        res = this->OnFail_ReturnTypeDefinition();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SPRV_NOT_THIS_RULE) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        _tprintf(TEXT("%snot this rule.\n"), this->tab);
        return SPRV_NOT_THIS_RULE;
    }
    else if(res == SPRV_EOF) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        this->removeTab();
        return SPRV_EOF;
    }
    else if(res != SPRV_CAN_CONTINUE) {
        this->removeTab();
        return res;
    }
    _tprintf(TEXT("%schecking ParametersListDeclaration_part0_inplace1_part0_inplace0\n"), this->tab);
    res = this->ParametersListDeclaration_part0_inplace1_part0_inplace0();
    if(res == SRV_SUCCESS) {
        finalRes = SRV_SUCCESS;
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        res = this->OnSuccess_ParametersListDeclaration_part0_inplace1_part0_inplace0();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        res = this->OnFail_ParametersListDeclaration_part0_inplace1_part0_inplace0();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SPRV_NOT_THIS_RULE) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        _tprintf(TEXT("%snot this rule.\n"), this->tab);
        return SPRV_NOT_THIS_RULE;
    }
    else if(res == SPRV_EOF) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        this->removeTab();
        return SPRV_EOF;
    }
    else if(res != SPRV_CAN_CONTINUE) {
        this->removeTab();
        return res;
    }

    this->removeTab();
    return finalRes;
}

// [ __sgrammar_identifier ] 
scriptRetCode scriptParser::ParametersListDeclaration_part0_inplace1_part0_inplace0() {

    scriptRetCode res;

    this->addTab();
    _tprintf(TEXT("%schecking ParametersListDeclaration_part0_inplace1_part0_inplace0_part0\n"), this->tab);
    res = this->ParametersListDeclaration_part0_inplace1_part0_inplace0_part0();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_ParametersListDeclaration_part0_inplace1_part0_inplace0_part0();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_ParametersListDeclaration_part0_inplace1_part0_inplace0_part0();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
        return res;
    }
    _tprintf(TEXT("%scan continue.\n"), this->tab);
    this->removeTab();
    return SPRV_CAN_CONTINUE;
}

// __sgrammar_identifier 
scriptRetCode scriptParser::ParametersListDeclaration_part0_inplace1_part0_inplace0_part0() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
	res = this->isId();
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }

    this->removeTab();
    return finalRes;
}

// ReturnTypeDefinition :: = void | bool | byte | char | int | long | float | string | QualifiedTypeName ; 
scriptRetCode scriptParser::ReturnTypeDefinition() {

    scriptRetCode res;

    this->addTab();
    _tprintf(TEXT("%schecking ReturnTypeDefinition_part0\n"), this->tab);
    res = this->ReturnTypeDefinition_part0();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_ReturnTypeDefinition_part0();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_ReturnTypeDefinition_part0();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
    }
    _tprintf(TEXT("%schecking ReturnTypeDefinition_part1\n"), this->tab);
    res = this->ReturnTypeDefinition_part1();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_ReturnTypeDefinition_part1();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_ReturnTypeDefinition_part1();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
    }
    _tprintf(TEXT("%schecking ReturnTypeDefinition_part2\n"), this->tab);
    res = this->ReturnTypeDefinition_part2();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_ReturnTypeDefinition_part2();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_ReturnTypeDefinition_part2();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
    }
    _tprintf(TEXT("%schecking ReturnTypeDefinition_part3\n"), this->tab);
    res = this->ReturnTypeDefinition_part3();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_ReturnTypeDefinition_part3();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_ReturnTypeDefinition_part3();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
    }
    _tprintf(TEXT("%schecking ReturnTypeDefinition_part4\n"), this->tab);
    res = this->ReturnTypeDefinition_part4();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_ReturnTypeDefinition_part4();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_ReturnTypeDefinition_part4();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
    }
    _tprintf(TEXT("%schecking ReturnTypeDefinition_part5\n"), this->tab);
    res = this->ReturnTypeDefinition_part5();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_ReturnTypeDefinition_part5();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_ReturnTypeDefinition_part5();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
    }
    _tprintf(TEXT("%schecking ReturnTypeDefinition_part6\n"), this->tab);
    res = this->ReturnTypeDefinition_part6();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_ReturnTypeDefinition_part6();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_ReturnTypeDefinition_part6();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
    }
    _tprintf(TEXT("%schecking ReturnTypeDefinition_part7\n"), this->tab);
    res = this->ReturnTypeDefinition_part7();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_ReturnTypeDefinition_part7();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_ReturnTypeDefinition_part7();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
    }
    _tprintf(TEXT("%schecking ReturnTypeDefinition_part8\n"), this->tab);
    res = this->ReturnTypeDefinition_part8();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_ReturnTypeDefinition_part8();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_ReturnTypeDefinition_part8();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
    }
    _tprintf(TEXT("%snot this rule.\n"), this->tab);
    this->removeTab();
    return SPRV_NOT_THIS_RULE;
}

// void 
scriptRetCode scriptParser::ReturnTypeDefinition_part0() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
	res = this->isRword( LRW_VOID );
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }

    this->removeTab();
    return finalRes;
}

// bool 
scriptRetCode scriptParser::ReturnTypeDefinition_part1() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
	res = this->isRword( LRW_BOOL );
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }

    this->removeTab();
    return finalRes;
}

// byte 
scriptRetCode scriptParser::ReturnTypeDefinition_part2() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
	res = this->isRword( LRW_BYTE );
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }

    this->removeTab();
    return finalRes;
}

// char 
scriptRetCode scriptParser::ReturnTypeDefinition_part3() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
	res = this->isRword( LRW_CHAR );
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }

    this->removeTab();
    return finalRes;
}

// int 
scriptRetCode scriptParser::ReturnTypeDefinition_part4() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
	res = this->isRword( LRW_INT );
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }

    this->removeTab();
    return finalRes;
}

// long 
scriptRetCode scriptParser::ReturnTypeDefinition_part5() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
	res = this->isRword( LRW_LONG );
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }

    this->removeTab();
    return finalRes;
}

// float 
scriptRetCode scriptParser::ReturnTypeDefinition_part6() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
	res = this->isRword( LRW_FLOAT );
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }

    this->removeTab();
    return finalRes;
}

// string 
scriptRetCode scriptParser::ReturnTypeDefinition_part7() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
	res = this->isRword( LRW_STRING );
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }

    this->removeTab();
    return finalRes;
}

// QualifiedTypeName 
scriptRetCode scriptParser::ReturnTypeDefinition_part8() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
    _tprintf(TEXT("%schecking QualifiedTypeName\n"), this->tab);
    res = this->QualifiedTypeName();
    if(res == SRV_SUCCESS) {
        finalRes = SRV_SUCCESS;
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        res = this->OnSuccess_QualifiedTypeName();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        res = this->OnFail_QualifiedTypeName();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SPRV_NOT_THIS_RULE) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        _tprintf(TEXT("%snot this rule.\n"), this->tab);
        return SPRV_NOT_THIS_RULE;
    }
    else if(res == SPRV_EOF) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        this->removeTab();
        return SPRV_EOF;
    }
    else if(res != SPRV_CAN_CONTINUE) {
        this->removeTab();
        return res;
    }

    this->removeTab();
    return finalRes;
}

// TypeModifier :: = { TypeModifier2 } ; 
scriptRetCode scriptParser::TypeModifier() {

    scriptRetCode res;

    this->addTab();
    _tprintf(TEXT("%schecking TypeModifier_part0\n"), this->tab);
    res = this->TypeModifier_part0();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_TypeModifier_part0();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_TypeModifier_part0();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
    }
    _tprintf(TEXT("%snot this rule.\n"), this->tab);
    this->removeTab();
    return SPRV_NOT_THIS_RULE;
}

// { TypeModifier2 } 
scriptRetCode scriptParser::TypeModifier_part0() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
    _tprintf(TEXT("%schecking TypeModifier_part0_inplace0\n"), this->tab);
    res = this->TypeModifier_part0_inplace0();
    if(res == SRV_SUCCESS) {
        finalRes = SRV_SUCCESS;
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        res = this->OnSuccess_TypeModifier_part0_inplace0();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        res = this->OnFail_TypeModifier_part0_inplace0();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SPRV_NOT_THIS_RULE) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        _tprintf(TEXT("%snot this rule.\n"), this->tab);
        return SPRV_NOT_THIS_RULE;
    }
    else if(res == SPRV_EOF) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        this->removeTab();
        return SPRV_EOF;
    }
    else if(res != SPRV_CAN_CONTINUE) {
        this->removeTab();
        return res;
    }

    this->removeTab();
    return finalRes;
}

// { TypeModifier2 } 
scriptRetCode scriptParser::TypeModifier_part0_inplace0() {

    scriptRetCode res;

    bool firstPart = true;
    this->addTab();
    while( true ) {
        _tprintf(TEXT("%schecking TypeModifier_part0_inplace0_part0\n"), this->tab);
        res = this->TypeModifier_part0_inplace0_part0();
        if(res == SRV_SUCCESS) {
            _tprintf(TEXT("%ssuccess.\n"), this->tab);
            this->OnSuccess_TypeModifier_part0_inplace0_part0();
            firstPart = false;
            continue;
        }
        else if(res == SRV_FAIL) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return this->OnFail_TypeModifier_part0_inplace0_part0();
        }
        else if(res != SPRV_NOT_THIS_RULE) {
            _tprintf(TEXT("%sanother res.\n"), this->tab);
            this->removeTab();
            return res;
        }
        break;
    }
    _tprintf(firstPart? TEXT("%scan continue.\n"): TEXT("%ssuccess.\n"), this->tab);
    this->removeTab();
    return firstPart? SPRV_CAN_CONTINUE: SRV_SUCCESS;
}

// TypeModifier2 
scriptRetCode scriptParser::TypeModifier_part0_inplace0_part0() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
    _tprintf(TEXT("%schecking TypeModifier2\n"), this->tab);
    res = this->TypeModifier2();
    if(res == SRV_SUCCESS) {
        finalRes = SRV_SUCCESS;
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        res = this->OnSuccess_TypeModifier2();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        res = this->OnFail_TypeModifier2();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SPRV_NOT_THIS_RULE) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        _tprintf(TEXT("%snot this rule.\n"), this->tab);
        return SPRV_NOT_THIS_RULE;
    }
    else if(res == SPRV_EOF) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        this->removeTab();
        return SPRV_EOF;
    }
    else if(res != SPRV_CAN_CONTINUE) {
        this->removeTab();
        return res;
    }

    this->removeTab();
    return finalRes;
}

// TypeModifier2 :: = new | public | protected | internal | virtual | private | abstract | sealed | static ; 
scriptRetCode scriptParser::TypeModifier2() {

    scriptRetCode res;

    this->addTab();
    _tprintf(TEXT("%schecking TypeModifier2_part0\n"), this->tab);
    res = this->TypeModifier2_part0();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_TypeModifier2_part0();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_TypeModifier2_part0();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
    }
    _tprintf(TEXT("%schecking TypeModifier2_part1\n"), this->tab);
    res = this->TypeModifier2_part1();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_TypeModifier2_part1();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_TypeModifier2_part1();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
    }
    _tprintf(TEXT("%schecking TypeModifier2_part2\n"), this->tab);
    res = this->TypeModifier2_part2();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_TypeModifier2_part2();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_TypeModifier2_part2();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
    }
    _tprintf(TEXT("%schecking TypeModifier2_part3\n"), this->tab);
    res = this->TypeModifier2_part3();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_TypeModifier2_part3();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_TypeModifier2_part3();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
    }
    _tprintf(TEXT("%schecking TypeModifier2_part4\n"), this->tab);
    res = this->TypeModifier2_part4();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_TypeModifier2_part4();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_TypeModifier2_part4();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
    }
    _tprintf(TEXT("%schecking TypeModifier2_part5\n"), this->tab);
    res = this->TypeModifier2_part5();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_TypeModifier2_part5();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_TypeModifier2_part5();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
    }
    _tprintf(TEXT("%schecking TypeModifier2_part6\n"), this->tab);
    res = this->TypeModifier2_part6();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_TypeModifier2_part6();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_TypeModifier2_part6();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
    }
    _tprintf(TEXT("%schecking TypeModifier2_part7\n"), this->tab);
    res = this->TypeModifier2_part7();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_TypeModifier2_part7();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_TypeModifier2_part7();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
    }
    _tprintf(TEXT("%schecking TypeModifier2_part8\n"), this->tab);
    res = this->TypeModifier2_part8();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_TypeModifier2_part8();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_TypeModifier2_part8();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
    }
    _tprintf(TEXT("%snot this rule.\n"), this->tab);
    this->removeTab();
    return SPRV_NOT_THIS_RULE;
}

// new 
scriptRetCode scriptParser::TypeModifier2_part0() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
	res = this->isRword( LRW_NEW );
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }

    this->removeTab();
    return finalRes;
}

// public 
scriptRetCode scriptParser::TypeModifier2_part1() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
	res = this->isRword( LRW_PUBLIC );
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }

    this->removeTab();
    return finalRes;
}

// protected 
scriptRetCode scriptParser::TypeModifier2_part2() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
	res = this->isRword( LRW_PROTECTED );
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }

    this->removeTab();
    return finalRes;
}

// internal 
scriptRetCode scriptParser::TypeModifier2_part3() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
	res = this->isRword( LRW_INTERNAL );
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }

    this->removeTab();
    return finalRes;
}

// virtual 
scriptRetCode scriptParser::TypeModifier2_part4() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
	res = this->isRword( LRW_VIRTUAL );
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }

    this->removeTab();
    return finalRes;
}

// private 
scriptRetCode scriptParser::TypeModifier2_part5() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
	res = this->isRword( LRW_PRIVATE );
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }

    this->removeTab();
    return finalRes;
}

// abstract 
scriptRetCode scriptParser::TypeModifier2_part6() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
	res = this->isRword( LRW_ABSTRACT );
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }

    this->removeTab();
    return finalRes;
}

// sealed 
scriptRetCode scriptParser::TypeModifier2_part7() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
	res = this->isRword( LRW_SEALED );
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }

    this->removeTab();
    return finalRes;
}

// static 
scriptRetCode scriptParser::TypeModifier2_part8() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
	res = this->isRword( LRW_STATIC );
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }

    this->removeTab();
    return finalRes;
}

// QualifiedTypeName :: = __sgrammar_identifier { "." __sgrammar_identifier } ; 
scriptRetCode scriptParser::QualifiedTypeName() {

    scriptRetCode res;

    this->addTab();
    _tprintf(TEXT("%schecking QualifiedTypeName_part0\n"), this->tab);
    res = this->QualifiedTypeName_part0();
    if(res == SRV_SUCCESS) {
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        this->removeTab();
        return this->OnSuccess_QualifiedTypeName_part0();
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        this->removeTab();
        return this->OnFail_QualifiedTypeName_part0();
    }
    else if(res != SPRV_NOT_THIS_RULE) {
        _tprintf(TEXT("%sanother res.\n"), this->tab);
        this->removeTab();
    }
    _tprintf(TEXT("%snot this rule.\n"), this->tab);
    this->removeTab();
    return SPRV_NOT_THIS_RULE;
}

// __sgrammar_identifier { "." __sgrammar_identifier } 
scriptRetCode scriptParser::QualifiedTypeName_part0() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
	res = this->isId();
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }
    _tprintf(TEXT("%schecking QualifiedTypeName_part0_inplace0\n"), this->tab);
    res = this->QualifiedTypeName_part0_inplace0();
    if(res == SRV_SUCCESS) {
        finalRes = SRV_SUCCESS;
        _tprintf(TEXT("%ssuccess.\n"), this->tab);
        res = this->OnSuccess_QualifiedTypeName_part0_inplace0();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SRV_FAIL) {
        _tprintf(TEXT("%sfail.\n"), this->tab);
        res = this->OnFail_QualifiedTypeName_part0_inplace0();
        if(res != SRV_SUCCESS) {
            this->removeTab();
            return res;
        }
    }
    else if(res == SPRV_NOT_THIS_RULE) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        _tprintf(TEXT("%snot this rule.\n"), this->tab);
        return SPRV_NOT_THIS_RULE;
    }
    else if(res == SPRV_EOF) {
        if(finalRes == SRV_SUCCESS) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return SRV_FAIL;
        }
        this->removeTab();
        return SPRV_EOF;
    }
    else if(res != SPRV_CAN_CONTINUE) {
        this->removeTab();
        return res;
    }

    this->removeTab();
    return finalRes;
}

// { "." __sgrammar_identifier } 
scriptRetCode scriptParser::QualifiedTypeName_part0_inplace0() {

    scriptRetCode res;

    bool firstPart = true;
    this->addTab();
    while( true ) {
        _tprintf(TEXT("%schecking QualifiedTypeName_part0_inplace0_part0\n"), this->tab);
        res = this->QualifiedTypeName_part0_inplace0_part0();
        if(res == SRV_SUCCESS) {
            _tprintf(TEXT("%ssuccess.\n"), this->tab);
            this->OnSuccess_QualifiedTypeName_part0_inplace0_part0();
            firstPart = false;
            continue;
        }
        else if(res == SRV_FAIL) {
            _tprintf(TEXT("%sfail.\n"), this->tab);
            this->removeTab();
            return this->OnFail_QualifiedTypeName_part0_inplace0_part0();
        }
        else if(res != SPRV_NOT_THIS_RULE) {
            _tprintf(TEXT("%sanother res.\n"), this->tab);
            this->removeTab();
            return res;
        }
        break;
    }
    _tprintf(firstPart? TEXT("%scan continue.\n"): TEXT("%ssuccess.\n"), this->tab);
    this->removeTab();
    return firstPart? SPRV_CAN_CONTINUE: SRV_SUCCESS;
}

// "." __sgrammar_identifier 
scriptRetCode scriptParser::QualifiedTypeName_part0_inplace0_part0() {

    scriptRetCode res;

    scriptRetCode finalRes = SPRV_CAN_CONTINUE;

    this->addTab();
    res = this->isParser( LPT_DOT );
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }
	res = this->isId();
    if(res == SRV_SUCCESS)
        finalRes = res;
    else {
        if(finalRes == SRV_SUCCESS) {
            this->removeTab();
            return SRV_FAIL;
        }
        return res;
    }

    this->removeTab();
    return finalRes;
}

scriptRetCode scriptParser::OnSuccess_enter_rule() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_enter_rule_part0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_Programm() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_Programm_part0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_Programm_part0_inplace0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_Programm_part0_inplace0_part0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_Programm_part0_inplace1() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_Programm_part0_inplace1_part0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_UsingDefinition() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_UsingDefinition_part0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_UsingDefinition_part0_inplace0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_UsingDefinition_part0_inplace0_part0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_NamespaceDefinition() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_NamespaceDefinition_part0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_NamespaceDefinitionBegin() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_NamespaceDefinitionBegin_part0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_NamespaceMembersDefinition() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_NamespaceMembersDefinition_part0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_NamespaceMembersDefinition_part0_inplace0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_NamespaceMembersDefinition_part0_inplace0_part0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_NamespaceMemberDefinition() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_NamespaceMemberDefinition_part0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_NamespaceMemberDefinition_part1() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_TypeDefinition() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_TypeDefinition_part0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_TypeDefinition_part0_inplace0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_TypeDefinition_part0_inplace0_part0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_TypeDefinition_part0_inplace0_part1() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_TypeDefinition_part0_inplace0_part2() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_EnumDefinition() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_EnumDefinition_part0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_EnumBodyDefinition() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_EnumBodyDefinition_part0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_EnumBodyDefinition_part0_inplace0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_EnumBodyDefinition_part0_inplace0_part0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_EnumItemDefinition() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_EnumItemDefinition_part0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_EnumItemDefinition_part0_inplace0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_EnumItemDefinition_part0_inplace0_part0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_ClassDefinition() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_ClassDefinition_part0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_InterfaceDefinition() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_InterfaceDefinition_part0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_InterfaceDefinition_part0_inplace0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_InterfaceDefinition_part0_inplace0_part0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_DirevedFromList() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_DirevedFromList_part0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_DirevedFromList_part0_inplace0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_DirevedFromList_part0_inplace0_part0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_InterfaceBody() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_InterfaceBody_part0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_InterfaceBody_part0_inplace0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_InterfaceBody_part0_inplace0_part0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_InterfaceBody_part0_inplace0_part0_inplace0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_InterfaceBody_part0_inplace0_part0_inplace0_part0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_InterfaceBody_part0_inplace0_part0_inplace0_part1() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_MethodOrPropertyDeclaration() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_MethodOrPropertyDeclaration_part0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_MethodDeclaration() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_MethodDeclaration_part0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_MethodDeclaration_part0_inplace0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_MethodDeclaration_part0_inplace0_part0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_PropertyDeclaration() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_PropertyDeclaration_part0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_PropertyDeclaration_part0_inplace0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_PropertyDeclaration_part0_inplace0_part0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_PropertyDeclaration_part0_inplace0_part1() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_GetThenSetInterfacePropDecl() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_GetThenSetInterfacePropDecl_part0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_GetThenSetInterfacePropDecl_part0_inplace0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_GetThenSetInterfacePropDecl_part0_inplace0_part0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_SetThemGetInterfacePropDecl() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_SetThemGetInterfacePropDecl_part0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_SetThemGetInterfacePropDecl_part0_inplace0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_SetThemGetInterfacePropDecl_part0_inplace0_part0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_MethodOrPropertyName() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_MethodOrPropertyName_part0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_ParametersListDeclaration() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_ParametersListDeclaration_part0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_ParametersListDeclaration_part0_inplace0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_ParametersListDeclaration_part0_inplace0_part0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_ParametersListDeclaration_part0_inplace1() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_ParametersListDeclaration_part0_inplace1_part0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_ParametersListDeclaration_part0_inplace1_part0_inplace0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_ParametersListDeclaration_part0_inplace1_part0_inplace0_part0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_ReturnTypeDefinition() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_ReturnTypeDefinition_part0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_ReturnTypeDefinition_part1() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_ReturnTypeDefinition_part2() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_ReturnTypeDefinition_part3() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_ReturnTypeDefinition_part4() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_ReturnTypeDefinition_part5() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_ReturnTypeDefinition_part6() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_ReturnTypeDefinition_part7() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_ReturnTypeDefinition_part8() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_TypeModifier() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_TypeModifier_part0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_TypeModifier_part0_inplace0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_TypeModifier_part0_inplace0_part0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_TypeModifier2() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_TypeModifier2_part0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_TypeModifier2_part1() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_TypeModifier2_part2() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_TypeModifier2_part3() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_TypeModifier2_part4() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_TypeModifier2_part5() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_TypeModifier2_part6() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_TypeModifier2_part7() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_TypeModifier2_part8() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_QualifiedTypeName() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_QualifiedTypeName_part0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_QualifiedTypeName_part0_inplace0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnSuccess_QualifiedTypeName_part0_inplace0_part0() {

	return SRV_SUCCESS;
}

scriptRetCode scriptParser::OnFail_enter_rule() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_enter_rule_part0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_Programm() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_Programm_part0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_Programm_part0_inplace0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_Programm_part0_inplace0_part0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_Programm_part0_inplace1() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_Programm_part0_inplace1_part0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_UsingDefinition() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_UsingDefinition_part0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_UsingDefinition_part0_inplace0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_UsingDefinition_part0_inplace0_part0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_NamespaceDefinition() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_NamespaceDefinition_part0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_NamespaceDefinitionBegin() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_NamespaceDefinitionBegin_part0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_NamespaceMembersDefinition() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_NamespaceMembersDefinition_part0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_NamespaceMembersDefinition_part0_inplace0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_NamespaceMembersDefinition_part0_inplace0_part0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_NamespaceMemberDefinition() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_NamespaceMemberDefinition_part0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_NamespaceMemberDefinition_part1() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_TypeDefinition() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_TypeDefinition_part0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_TypeDefinition_part0_inplace0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_TypeDefinition_part0_inplace0_part0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_TypeDefinition_part0_inplace0_part1() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_TypeDefinition_part0_inplace0_part2() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_EnumDefinition() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_EnumDefinition_part0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_EnumBodyDefinition() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_EnumBodyDefinition_part0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_EnumBodyDefinition_part0_inplace0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_EnumBodyDefinition_part0_inplace0_part0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_EnumItemDefinition() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_EnumItemDefinition_part0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_EnumItemDefinition_part0_inplace0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_EnumItemDefinition_part0_inplace0_part0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_ClassDefinition() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_ClassDefinition_part0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_InterfaceDefinition() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_InterfaceDefinition_part0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_InterfaceDefinition_part0_inplace0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_InterfaceDefinition_part0_inplace0_part0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_DirevedFromList() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_DirevedFromList_part0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_DirevedFromList_part0_inplace0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_DirevedFromList_part0_inplace0_part0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_InterfaceBody() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_InterfaceBody_part0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_InterfaceBody_part0_inplace0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_InterfaceBody_part0_inplace0_part0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_InterfaceBody_part0_inplace0_part0_inplace0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_InterfaceBody_part0_inplace0_part0_inplace0_part0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_InterfaceBody_part0_inplace0_part0_inplace0_part1() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_MethodOrPropertyDeclaration() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_MethodOrPropertyDeclaration_part0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_MethodDeclaration() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_MethodDeclaration_part0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_MethodDeclaration_part0_inplace0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_MethodDeclaration_part0_inplace0_part0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_PropertyDeclaration() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_PropertyDeclaration_part0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_PropertyDeclaration_part0_inplace0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_PropertyDeclaration_part0_inplace0_part0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_PropertyDeclaration_part0_inplace0_part1() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_GetThenSetInterfacePropDecl() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_GetThenSetInterfacePropDecl_part0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_GetThenSetInterfacePropDecl_part0_inplace0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_GetThenSetInterfacePropDecl_part0_inplace0_part0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_SetThemGetInterfacePropDecl() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_SetThemGetInterfacePropDecl_part0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_SetThemGetInterfacePropDecl_part0_inplace0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_SetThemGetInterfacePropDecl_part0_inplace0_part0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_MethodOrPropertyName() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_MethodOrPropertyName_part0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_ParametersListDeclaration() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_ParametersListDeclaration_part0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_ParametersListDeclaration_part0_inplace0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_ParametersListDeclaration_part0_inplace0_part0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_ParametersListDeclaration_part0_inplace1() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_ParametersListDeclaration_part0_inplace1_part0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_ParametersListDeclaration_part0_inplace1_part0_inplace0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_ParametersListDeclaration_part0_inplace1_part0_inplace0_part0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_ReturnTypeDefinition() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_ReturnTypeDefinition_part0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_ReturnTypeDefinition_part1() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_ReturnTypeDefinition_part2() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_ReturnTypeDefinition_part3() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_ReturnTypeDefinition_part4() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_ReturnTypeDefinition_part5() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_ReturnTypeDefinition_part6() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_ReturnTypeDefinition_part7() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_ReturnTypeDefinition_part8() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_TypeModifier() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_TypeModifier_part0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_TypeModifier_part0_inplace0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_TypeModifier_part0_inplace0_part0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_TypeModifier2() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_TypeModifier2_part0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_TypeModifier2_part1() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_TypeModifier2_part2() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_TypeModifier2_part3() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_TypeModifier2_part4() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_TypeModifier2_part5() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_TypeModifier2_part6() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_TypeModifier2_part7() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_TypeModifier2_part8() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_QualifiedTypeName() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_QualifiedTypeName_part0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_QualifiedTypeName_part0_inplace0() {

	return SRV_FAIL;
}

scriptRetCode scriptParser::OnFail_QualifiedTypeName_part0_inplace0_part0() {

	return SRV_FAIL;
}

