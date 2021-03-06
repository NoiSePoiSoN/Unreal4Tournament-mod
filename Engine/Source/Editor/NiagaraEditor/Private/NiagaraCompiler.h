// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "INiagaraCompiler.h"
#include "CompilerResultsLog.h"
#include "NiagaraScriptConstantData.h"

/** Base class for Niagara compilers. Children of this will include a compiler for the VectorVM and for Compute shaders. Possibly others. */
class NIAGARAEDITOR_API FNiagaraCompiler : public INiagaraCompiler
{
protected:
	/** The script we are compiling. */
	UNiagaraScript* Script;

	/** The source of the script we are compiling. */
	UNiagaraScriptSource* Source;

	/** The source graph of the script we are compiling. */
	UNiagaraGraph* SourceGraph;

	/** The set of expressions generated from the script source. */
	TArray<TNiagaraExprPtr> Expressions;

	//All internal and external constants used in the graph.
	FNiagaraScriptConstantData ConstantData;

	/** Map of Pins to expressions. Allows us to reuse expressions for pins that have already been compiled. */
	TMap<UEdGraphPin*, TNiagaraExprPtr> PinToExpression;

	//Message log. Automatically handles marking the NodeGraph with errors.
	FCompilerResultsLog MessageLog;

	/** Compiles an output Pin on a graph node. Caches the result for any future inputs connected to it. */
	TNiagaraExprPtr CompileOutputPin(UEdGraphPin* Pin);

	/** Creates an expression retrieving a particle attribute. */
	TNiagaraExprPtr Expression_GetAttribute(const FNiagaraVariableInfo& Attribute);

	/** Creates an expression retrieving an external constant. */
	TNiagaraExprPtr Expression_GetExternalConstant(const FNiagaraVariableInfo& Constant);

	/** Creates an expression retrieving an internal constant. */
	TNiagaraExprPtr Expression_GetInternalConstant(const FNiagaraVariableInfo& Constant);

	/** Creates an expression collecting some source expressions together for use by future expressions. */
	TNiagaraExprPtr Expression_Collection(TArray<TNiagaraExprPtr>& SourceExpressions);

	/** Sets the value of a constant. Overwrites the value if it already exists. */
	template< typename T >
	void SetOrAddConstant(bool bInternal, const FNiagaraVariableInfo& Constant, const T& Default);

	/** Searches for Function Call nodes and merges their graphs into the main graph. */
	bool MergeInFunctionNodes();

public:

	//Begin INiagaraCompiler Interface
	virtual TNiagaraExprPtr CompilePin(UEdGraphPin* Pin)override;

	virtual TNiagaraExprPtr GetAttribute(const FNiagaraVariableInfo& Attribute)override;
	virtual TNiagaraExprPtr GetExternalConstant(const FNiagaraVariableInfo& Constant, float Default)override;
	virtual TNiagaraExprPtr GetExternalConstant(const FNiagaraVariableInfo& Constant, const FVector4& Default)override;
	virtual TNiagaraExprPtr GetExternalConstant(const FNiagaraVariableInfo& Constant, const FMatrix& Default)override;
	virtual TNiagaraExprPtr GetInternalConstant(const FNiagaraVariableInfo& Constant, float Default)override;
	virtual TNiagaraExprPtr GetInternalConstant(const FNiagaraVariableInfo& Constant, const FVector4& Default)override;
	virtual TNiagaraExprPtr GetInternalConstant(const FNiagaraVariableInfo& Constant, const FMatrix& Default)override;
	virtual TNiagaraExprPtr GetExternalCurveConstant(const FNiagaraVariableInfo& Constant)override;

	virtual void CheckInputs(FName OpName, TArray<TNiagaraExprPtr>& Inputs)override;
	virtual void CheckOutputs(FName OpName, TArray<TNiagaraExprPtr>& Outputs)override;
	//End INiagaraCompiler Interface
	
	/** Gets the index into a constants table of the constant specified by Name and bInternal. */
	virtual ENiagaraDataType GetConstantResultIndex(const FNiagaraVariableInfo& Constant, bool bInternal, int32& OutResultIndex, int32& OutComponentIndex) = 0;
	/**	Gets the index of an attribute. */
	int32 GetAttributeIndex(const FNiagaraVariableInfo& Attr)const;
	virtual void GetParticleAttributes(TArray<FNiagaraVariableInfo>& OutAttributes)const;

	/**	Gets the index of a free temporary location. */
	virtual int32 AquireTemporary() = 0;
	/** Frees the passed temporary index for use by other expressions. */
	virtual void FreeTemporary(int32 TempIndex) = 0;
};

template<typename T>
void FNiagaraCompiler::SetOrAddConstant(bool bInternal, const FNiagaraVariableInfo& Constant, const T& Default)
{
	if (bInternal)
		ConstantData.SetOrAddInternal(Constant, Default);
	else
		ConstantData.SetOrAddExternal(Constant, Default);
}
