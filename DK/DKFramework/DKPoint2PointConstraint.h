//
//  File: DKPoint2PointConstraint.h
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2016 Hongtae Kim. All rights reserved.
//

#pragma once
#include "../DKFoundation.h"
#include "DKConstraint.h"

namespace DKFramework
{
	/// @brief point to point constraint between two rigid bodies each with a pivot.
	class DKGL_API DKPoint2PointConstraint : public DKConstraint
	{
	public:
		DKPoint2PointConstraint(DKRigidBody* bodyA, DKRigidBody* bodyB, const DKVector3& pivotInA, const DKVector3& pivotInB);
		DKPoint2PointConstraint(DKRigidBody* bodyA, const DKVector3& pivotInA);
		DKPoint2PointConstraint();
		~DKPoint2PointConstraint();

		DKVector3 PivotInA() const;
		DKVector3 PivotInB() const;
		void SetPivotInA(const DKVector3& v);
		void SetPivotInB(const DKVector3& v);

		bool IsValidParam(ParamType type, ParamAxis axis) const override;
		bool HasParam(ParamType type, ParamAxis axis) const override;

		DKObject<DKSerializer> Serializer() override;

	protected:
		void ResetContext() override;
		void ResetContextImpl() override;

		DKObject<DKModel> Clone(UUIDObjectMap&) const override;
		DKPoint2PointConstraint* Copy(UUIDObjectMap&, const DKPoint2PointConstraint*);
	};
}
