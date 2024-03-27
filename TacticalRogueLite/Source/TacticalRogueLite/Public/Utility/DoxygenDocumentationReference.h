// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class TACTICALROGUELITE_API DoxygenDocumentationReference
{
private:
	/*!
	 *This function has no return values. And is private.
	 */
	UFUNCTION()
	void VoidNoParams_Private();
public:
	/*!
	 *This function has no return values. And is public.
	 */
	UFUNCTION()
	void VoidNoParams();
	/*!
	 *This function has one parameter.
	 *@param Parameter This parameter does stuff.
	 */
	UFUNCTION()
	void VoidOneParams(int Parameter);
	/*!
	 *This is a function with a return value
	 *@param String this string will be altered.
	 *@return This returns the altered string
	 */
	UFUNCTION()
	FString ReturnStringOneParam(FString String);

	/*! \brief This is a brief description of the elaborate function.
	 *
	 *	This is a much longer description that takes way to much space to be displayed in the brief description field which is why we're using the brief description instead!
	 */
	UFUNCTION()
	void VeryElaborateFunction();

	/*! This is some documentation for a variable.*/
	int Variable;
	
	int CoolerVariable; /*!< This documentation is after the variable and is such, much cooler*/
	int BriefVar; //!< This is a brief description;

	/*!
	 *This function requires you to look at another function.
	 *@see VeryElaborateFunction()
	 */
	UFUNCTION()
	void ReferenceFunction();

	/*! @copybrief VeryElaborateFunction
	 *
	 *This is the actual description
	 *@param a This is an Integer
	 *@param b This is a String
	 *@return This returns a String
	 *@attention ATTENTION
	 *@see ReferenceFunction()
	 *@bug This is a bug.
	 *@author Theo
	 */
	FString ThisIsATest(int a,FString b);
	DoxygenDocumentationReference();
	~DoxygenDocumentationReference();
};
