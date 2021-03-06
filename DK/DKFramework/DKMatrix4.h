//
//  File: DKMatrix4.h
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2016 Hongtae Kim. All rights reserved.
//

#pragma once
#include "../DKFoundation.h"

#pragma pack(push, 4)
namespace DKFramework
{
	class DKVector4;
	/// @brief 4x4 matrix
	///
	/// @note
	///   This matrix order is Row-major.
	///   transform of Vector4 V is V' = V * Matrix
	class DKGL_API DKMatrix4
	{
	public:
		DKMatrix4();
		DKMatrix4(const DKVector4& row1, const DKVector4& row2, const DKVector4& row3, const DKVector4& row4);
		DKMatrix4(float e11, float e12, float e13, float e14, float e21, float e22, float e23, float e24,
			float e31, float e32, float e33, float e34, float e41, float e42, float e43, float e44);

		DKMatrix4& SetIdentity();
		bool IsIdentity() const;
		bool IsDiagonal() const;

		DKMatrix4& Inverse(bool* result = nullptr);
		DKMatrix4& Transpose();
		DKMatrix4& Multiply(const DKMatrix4& m);

		float Determinant() const;
        DKMatrix4 InverseMatrix(bool* result = nullptr, float* determinant = nullptr) const;
        DKMatrix4 TransposeMatrix() const;

		DKMatrix4 operator * (const DKMatrix4& m) const;
		DKMatrix4 operator + (const DKMatrix4& m) const;
		DKMatrix4 operator - (const DKMatrix4& m) const;
		DKMatrix4 operator * (float f) const;
		DKMatrix4 operator / (float f) const;

		DKMatrix4& operator *= (const DKMatrix4& m);
		DKMatrix4& operator += (const DKMatrix4& m);
		DKMatrix4& operator -= (const DKMatrix4& m);
		DKMatrix4& operator *= (float f);
		DKMatrix4& operator /= (float f);

		bool operator == (const DKMatrix4& m) const;
		bool operator != (const DKMatrix4& m) const;

		DKVector4 Row1() const;
		DKVector4 Row2() const;
		DKVector4 Row3() const;
		DKVector4 Row4() const;
		DKVector4 Column1() const;
		DKVector4 Column2() const;
		DKVector4 Column3() const;
		DKVector4 Column4() const;

		union
		{
			struct
			{
				float _11, _12, _13, _14;
				float _21, _22, _23, _24;
				float _31, _32, _33, _34;
				float _41, _42, _43, _44;
			};
			struct
			{
				float m[4][4];
			};
			float val[16];
		};

		static const DKMatrix4 identity;
	};
}
#pragma pack(pop)
