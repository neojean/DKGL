//
//  File: DKMatrix3.h
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2016 Hongtae Kim. All rights reserved.
//

#pragma once
#include "../DKFoundation.h"

#pragma pack(push, 4)
namespace DKFramework
{
	class DKVector3;
	/// @brief 3x3 matrix
	///
	/// @note
	///   This matrix order is Row-major.
	///   transform of Vector3 V is V' = V * Matrix
	class DKGL_API DKMatrix3
	{
	public:
		DKMatrix3();
		DKMatrix3(const DKVector3& row1, const DKVector3& row2, const DKVector3& row3);
		DKMatrix3(float e11, float e12, float e13, float e21, float e22, float e23, float e31, float e32, float e33);

		DKMatrix3& Zero();
		DKMatrix3& Identity();
		bool IsIdentity() const;
		bool IsDiagonal() const;

		DKMatrix3& Inverse();
		DKMatrix3& Transpose();
		DKMatrix3& Multiply(const DKMatrix3& m);

		float Determinant() const;
		bool GetInverseMatrix(DKMatrix3& matOut, float *pDeterminant) const;

		DKMatrix3 operator * (const DKMatrix3& m) const;
		DKMatrix3 operator + (const DKMatrix3& m) const;
		DKMatrix3 operator - (const DKMatrix3& m) const;
		DKMatrix3 operator * (float f) const;
		DKMatrix3 operator / (float f) const;

		DKMatrix3& operator *= (const DKMatrix3& m);
		DKMatrix3& operator += (const DKMatrix3& m);
		DKMatrix3& operator -= (const DKMatrix3& m);
		DKMatrix3& operator *= (float f);
		DKMatrix3& operator /= (float f);

		bool operator == (const DKMatrix3& m) const;
		bool operator != (const DKMatrix3& m) const;

		DKVector3 Row1() const;
		DKVector3 Row2() const;
		DKVector3 Row3() const;
		DKVector3 Column1() const;
		DKVector3 Column2() const;
		DKVector3 Column3() const;

		union
		{
			struct
			{
				float _11, _12, _13;
				float _21, _22, _23;
				float _31, _32, _33;
			};
			struct
			{
				float m[3][3];
			};
			float val[9];
		};

		static const DKMatrix3 identity;
	};
}
#pragma pack(pop)
