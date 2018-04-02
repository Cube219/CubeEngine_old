#pragma once

#include "Vector.h"

namespace cube
{
	class Matrix
	{
	public:
		static Matrix Zero()
		{
			Matrix r;
			r.mRows[0] = VectorBase::Zero();
			r.mRows[0] = VectorBase::Zero();
			r.mRows[0] = VectorBase::Zero();
			r.mRows[0] = VectorBase::Zero();

			return r;
		}
		static Matrix Identity()
		{
			Matrix r;
			r.mRows[0] = VectorBase(1.0f, 0.0f, 0.0f, 0.0f);
			r.mRows[1] = VectorBase(0.0f, 1.0f, 0.0f, 0.0f);
			r.mRows[2] = VectorBase(0.0f, 0.0f, 1.0f, 0.0f);
			r.mRows[3] = VectorBase(0.0f, 0.0f, 0.0f, 1.0f);
		}

		Matrix()
		{
		}
		Matrix(const VectorBase& row1, const VectorBase& row2, const VectorBase& row3, const VectorBase& row4) :
			mRows{row1, row2, row3, row4}
		{
		}
		Matrix(float v[16])
		{
			mRows[0] = VectorBase(v[0], v[1], v[2], v[3]);
			mRows[1] = VectorBase(v[4], v[5], v[6], v[7]);
			mRows[2] = VectorBase(v[8], v[9], v[10], v[11]);
			mRows[3] = VectorBase(v[12], v[13], v[14], v[15]);
		}
		Matrix(float v11, float v12, float v13, float v14,
			float v21, float v22, float v23, float v24,
			float v31, float v32, float v33, float v34,
			float v41, float v42, float v43, float v44)
		{
			mRows[0] = VectorBase(v11, v12, v13, v14);
			mRows[1] = VectorBase(v21, v22, v23, v24);
			mRows[2] = VectorBase(v31, v32, v33, v34);
			mRows[3] = VectorBase(v41, v42, v43, v44);
		}

		~Matrix()
		{
		}

		Matrix(const Matrix& other)
		{
			mRows[0] = other.mRows[0];
			mRows[1] = other.mRows[1];
			mRows[2] = other.mRows[2];
			mRows[3] = other.mRows[3];
		}
		Matrix& operator=(const Matrix& rhs)
		{
			mRows[0] = rhs.mRows[0];
			mRows[1] = rhs.mRows[1];
			mRows[2] = rhs.mRows[2];
			mRows[3] = rhs.mRows[3];

			return *this;
		}
		VectorBase& operator[](int i)
		{
			return mRows[i];
		}

		Matrix operator+ (const Matrix& rhs) const
		{
			Matrix r;
			r.mRows[0] = mRows[0] + rhs.mRows[0];
			r.mRows[1] = mRows[1] + rhs.mRows[1];
			r.mRows[2] = mRows[2] + rhs.mRows[2];
			r.mRows[3] = mRows[3] + rhs.mRows[3];

			return r;
		}
		Matrix operator- (const Matrix& rhs) const
		{
			Matrix r;
			r.mRows[0] = mRows[0] - rhs.mRows[0];
			r.mRows[1] = mRows[1] - rhs.mRows[1];
			r.mRows[2] = mRows[2] - rhs.mRows[2];
			r.mRows[3] = mRows[3] - rhs.mRows[3];

			return r;
		}
		Matrix operator* (float rhs) const
		{
			Matrix r;
			r.mRows[0] = mRows[0] * rhs;
			r.mRows[1] = mRows[1] * rhs;
			r.mRows[2] = mRows[2] * rhs;
			r.mRows[3] = mRows[3] * rhs;

			return r;
		}
		Matrix operator* (const Matrix& rhs) const
		{
			Matrix res = *this;
			res *= rhs;

			return res;
		}
		Matrix operator/ (float rhs) const
		{
			Matrix r;
			r.mRows[0] = mRows[0] / rhs;
			r.mRows[1] = mRows[1] / rhs;
			r.mRows[2] = mRows[2] / rhs;
			r.mRows[3] = mRows[3] / rhs;

			return r;
		}

		Matrix& operator+= (const Matrix& rhs)
		{
			mRows[0] += rhs.mRows[0];
			mRows[1] += rhs.mRows[1];
			mRows[2] += rhs.mRows[2];
			mRows[3] += rhs.mRows[3];

			return *this;
		}
		Matrix& operator-= (const Matrix& rhs)
		{
			mRows[0] -= rhs.mRows[0];
			mRows[1] -= rhs.mRows[1];
			mRows[2] -= rhs.mRows[2];
			mRows[3] -= rhs.mRows[3];

			return *this;
		}
		Matrix& operator*= (float rhs)
		{
			mRows[0] *= rhs;
			mRows[1] *= rhs;
			mRows[2] *= rhs;
			mRows[3] *= rhs;

			return *this;
		}
		Matrix& operator*= (const Matrix& rhs)
		{
#if defined (SIMD_SSE)
			VectorData v0, v1, v2, v3;

			// Row 0
			v3 = mRows[0].mData;
			v0 = _mm_shuffle_ps(v3, v3, _MM_SHUFFLE(0, 0, 0, 0));
			v1 = _mm_shuffle_ps(v3, v3, _MM_SHUFFLE(1, 1, 1, 1));
			v2 = _mm_shuffle_ps(v3, v3, _MM_SHUFFLE(2, 2, 2, 2));
			v3 = _mm_shuffle_ps(v3, v3, _MM_SHUFFLE(3, 3, 3, 3));

			v0 = _mm_mul_ps(v0, rhs.mRows[0].mData);
			v1 = _mm_mul_ps(v1, rhs.mRows[1].mData);
			v2 = _mm_mul_ps(v2, rhs.mRows[2].mData);
			v3 = _mm_mul_ps(v3, rhs.mRows[3].mData);

			v2 = _mm_add_ps(v2, v3);
			v1 = _mm_add_ps(v1, v2);
			v0 = _mm_add_ps(v0, v1);
			mRows[0].mData = v0;
			// Row 1
			v3 = mRows[1].mData;
			v0 = _mm_shuffle_ps(v3, v3, _MM_SHUFFLE(0, 0, 0, 0));
			v1 = _mm_shuffle_ps(v3, v3, _MM_SHUFFLE(1, 1, 1, 1));
			v2 = _mm_shuffle_ps(v3, v3, _MM_SHUFFLE(2, 2, 2, 2));
			v3 = _mm_shuffle_ps(v3, v3, _MM_SHUFFLE(3, 3, 3, 3));

			v0 = _mm_mul_ps(v0, rhs.mRows[0].mData);
			v1 = _mm_mul_ps(v1, rhs.mRows[1].mData);
			v2 = _mm_mul_ps(v2, rhs.mRows[2].mData);
			v3 = _mm_mul_ps(v3, rhs.mRows[3].mData);

			v2 = _mm_add_ps(v2, v3);
			v1 = _mm_add_ps(v1, v2);
			v0 = _mm_add_ps(v0, v1);
			mRows[1].mData = v0;
			// Row 2
			v3 = mRows[2].mData;
			v0 = _mm_shuffle_ps(v3, v3, _MM_SHUFFLE(0, 0, 0, 0));
			v1 = _mm_shuffle_ps(v3, v3, _MM_SHUFFLE(1, 1, 1, 1));
			v2 = _mm_shuffle_ps(v3, v3, _MM_SHUFFLE(2, 2, 2, 2));
			v3 = _mm_shuffle_ps(v3, v3, _MM_SHUFFLE(3, 3, 3, 3));

			v0 = _mm_mul_ps(v0, rhs.mRows[0].mData);
			v1 = _mm_mul_ps(v1, rhs.mRows[1].mData);
			v2 = _mm_mul_ps(v2, rhs.mRows[2].mData);
			v3 = _mm_mul_ps(v3, rhs.mRows[3].mData);

			v2 = _mm_add_ps(v2, v3);
			v1 = _mm_add_ps(v1, v2);
			v0 = _mm_add_ps(v0, v1);
			mRows[2].mData = v0;
			// Row 3
			v3 = mRows[3].mData;
			v0 = _mm_shuffle_ps(v3, v3, _MM_SHUFFLE(0, 0, 0, 0));
			v1 = _mm_shuffle_ps(v3, v3, _MM_SHUFFLE(1, 1, 1, 1));
			v2 = _mm_shuffle_ps(v3, v3, _MM_SHUFFLE(2, 2, 2, 2));
			v3 = _mm_shuffle_ps(v3, v3, _MM_SHUFFLE(3, 3, 3, 3));

			v0 = _mm_mul_ps(v0, rhs.mRows[0].mData);
			v1 = _mm_mul_ps(v1, rhs.mRows[1].mData);
			v2 = _mm_mul_ps(v2, rhs.mRows[2].mData);
			v3 = _mm_mul_ps(v3, rhs.mRows[3].mData);

			v2 = _mm_add_ps(v2, v3);
			v1 = _mm_add_ps(v1, v2);
			v0 = _mm_add_ps(v0, v1);
			mRows[3].mData = v0;
#elif defined(SIMD_NONE)
			float v0, v1, v2, v3;

			// Row 0
			v0 = mRows[0].mData[0];
			v1 = mRows[0].mData[1];
			v2 = mRows[0].mData[2];
			v3 = mRows[0].mData[3];

			mRows[0].mData[0] = (v0*rhs.mRows[0].mData[0]) + (v1*rhs.mRows[1].mData[0]) + (v2*rhs.mRows[2].mData[0]) + (v3*rhs.mRows[3].mData[0]);
			mRows[0].mData[1] = (v0*rhs.mRows[0].mData[1]) + (v1*rhs.mRows[1].mData[1]) + (v2*rhs.mRows[2].mData[1]) + (v3*rhs.mRows[3].mData[1]);
			mRows[0].mData[2] = (v0*rhs.mRows[0].mData[2]) + (v1*rhs.mRows[1].mData[2]) + (v2*rhs.mRows[2].mData[2]) + (v3*rhs.mRows[3].mData[2]);
			mRows[0].mData[3] = (v0*rhs.mRows[0].mData[3]) + (v1*rhs.mRows[1].mData[3]) + (v2*rhs.mRows[2].mData[3]) + (v3*rhs.mRows[3].mData[3]);

			// Row 1
			v0 = mRows[1].mData[0];
			v1 = mRows[1].mData[1];
			v2 = mRows[1].mData[2];
			v3 = mRows[1].mData[3];

			mRows[1].mData[0] = (v0*rhs.mRows[0].mData[0]) + (v1*rhs.mRows[1].mData[0]) + (v2*rhs.mRows[2].mData[0]) + (v3*rhs.mRows[3].mData[0]);
			mRows[1].mData[1] = (v0*rhs.mRows[0].mData[1]) + (v1*rhs.mRows[1].mData[1]) + (v2*rhs.mRows[2].mData[1]) + (v3*rhs.mRows[3].mData[1]);
			mRows[1].mData[2] = (v0*rhs.mRows[0].mData[2]) + (v1*rhs.mRows[1].mData[2]) + (v2*rhs.mRows[2].mData[2]) + (v3*rhs.mRows[3].mData[2]);
			mRows[1].mData[3] = (v0*rhs.mRows[0].mData[3]) + (v1*rhs.mRows[1].mData[3]) + (v2*rhs.mRows[2].mData[3]) + (v3*rhs.mRows[3].mData[3]);

			// Row 2
			v0 = mRows[2].mData[0];
			v1 = mRows[2].mData[1];
			v2 = mRows[2].mData[2];
			v3 = mRows[2].mData[3];

			mRows[2].mData[0] = (v0*rhs.mRows[0].mData[0]) + (v1*rhs.mRows[1].mData[0]) + (v2*rhs.mRows[2].mData[0]) + (v3*rhs.mRows[3].mData[0]);
			mRows[2].mData[1] = (v0*rhs.mRows[0].mData[1]) + (v1*rhs.mRows[1].mData[1]) + (v2*rhs.mRows[2].mData[1]) + (v3*rhs.mRows[3].mData[1]);
			mRows[2].mData[2] = (v0*rhs.mRows[0].mData[2]) + (v1*rhs.mRows[1].mData[2]) + (v2*rhs.mRows[2].mData[2]) + (v3*rhs.mRows[3].mData[2]);
			mRows[2].mData[3] = (v0*rhs.mRows[0].mData[3]) + (v1*rhs.mRows[1].mData[3]) + (v2*rhs.mRows[2].mData[3]) + (v3*rhs.mRows[3].mData[3]);

			// Row 3
			v0 = mRows[3].mData[0];
			v1 = mRows[3].mData[1];
			v2 = mRows[3].mData[2];
			v3 = mRows[3].mData[3];

			mRows[3].mData[0] = (v0*rhs.mRows[0].mData[0]) + (v1*rhs.mRows[1].mData[0]) + (v2*rhs.mRows[2].mData[0]) + (v3*rhs.mRows[3].mData[0]);
			mRows[3].mData[1] = (v0*rhs.mRows[0].mData[1]) + (v1*rhs.mRows[1].mData[1]) + (v2*rhs.mRows[2].mData[1]) + (v3*rhs.mRows[3].mData[1]);
			mRows[3].mData[2] = (v0*rhs.mRows[0].mData[2]) + (v1*rhs.mRows[1].mData[2]) + (v2*rhs.mRows[2].mData[2]) + (v3*rhs.mRows[3].mData[2]);
			mRows[3].mData[3] = (v0*rhs.mRows[0].mData[3]) + (v1*rhs.mRows[1].mData[3]) + (v2*rhs.mRows[2].mData[3]) + (v3*rhs.mRows[3].mData[3]);
#endif
			return *this;
		}
		Matrix& operator/= (float rhs)
		{
			mRows[0] /= rhs;
			mRows[1] /= rhs;
			mRows[2] /= rhs;
			mRows[3] /= rhs;

			return *this;
		}

		VectorBase GetCol(int index) const
		{
			VectorBase col;

#if defined (SIMD_SSE)
			VectorData t0, t1, t2, t3;

			switch(index) {
				case 0:
					t0 = _mm_shuffle_ps(mRows[0].mData, mRows[0].mData, _MM_SHUFFLE(0, 0, 0, 0));
					t1 = _mm_shuffle_ps(mRows[1].mData, mRows[1].mData, _MM_SHUFFLE(0, 0, 0, 0));
					t2 = _mm_shuffle_ps(mRows[2].mData, mRows[2].mData, _MM_SHUFFLE(0, 0, 0, 0));
					t3 = _mm_shuffle_ps(mRows[3].mData, mRows[3].mData, _MM_SHUFFLE(0, 0, 0, 0));
					break;
				case 1:
					t0 = _mm_shuffle_ps(mRows[0].mData, mRows[0].mData, _MM_SHUFFLE(1, 1, 1, 1));
					t1 = _mm_shuffle_ps(mRows[1].mData, mRows[1].mData, _MM_SHUFFLE(1, 1, 1, 1));
					t2 = _mm_shuffle_ps(mRows[2].mData, mRows[2].mData, _MM_SHUFFLE(1, 1, 1, 1));
					t3 = _mm_shuffle_ps(mRows[3].mData, mRows[3].mData, _MM_SHUFFLE(1, 1, 1, 1));
					break;
				case 2:
					t0 = _mm_shuffle_ps(mRows[0].mData, mRows[0].mData, _MM_SHUFFLE(2, 2, 2, 2));
					t1 = _mm_shuffle_ps(mRows[1].mData, mRows[1].mData, _MM_SHUFFLE(2, 2, 2, 2));
					t2 = _mm_shuffle_ps(mRows[2].mData, mRows[2].mData, _MM_SHUFFLE(2, 2, 2, 2));
					t3 = _mm_shuffle_ps(mRows[3].mData, mRows[3].mData, _MM_SHUFFLE(2, 2, 2, 2));
					break;
				case 3:
					t0 = _mm_shuffle_ps(mRows[0].mData, mRows[0].mData, _MM_SHUFFLE(3, 3, 3, 3));
					t1 = _mm_shuffle_ps(mRows[1].mData, mRows[1].mData, _MM_SHUFFLE(3, 3, 3, 3));
					t2 = _mm_shuffle_ps(mRows[2].mData, mRows[2].mData, _MM_SHUFFLE(3, 3, 3, 3));
					t3 = _mm_shuffle_ps(mRows[3].mData, mRows[3].mData, _MM_SHUFFLE(3, 3, 3, 3));
					break;
			}

			t3 = _mm_move_ss(t3, t2);
			t1 = _mm_move_ss(t1, t0);

			col.mData = _mm_movelh_ps(t1, t3);
#elif defined(SIMD_NONE)
			col.mData[0] = mRows[0].mData[index];
			col.mData[1] = mRows[1].mData[index];
			col.mData[2] = mRows[2].mData[index];
			col.mData[3] = mRows[3].mData[index];
#endif

			return col;
		}
		VectorBase GetRow(int index) const
		{
			return mRows[index];
		}

		void SetCol(int index, const VectorBase& col)
		{
#if defined(SIMD_SSE)
			VectorData t;

			if(index == 0) {
				mRows[0].mData = _mm_move_ss(mRows[0].mData, col.mData);
				mRows[1].mData = _mm_move_ss(mRows[1].mData, col.mData);
				mRows[2].mData = _mm_move_ss(mRows[2].mData, col.mData);
				mRows[3].mData = _mm_move_ss(mRows[3].mData, col.mData);

				return;
			}

			// row 0
			t = _mm_shuffle_ps(col.mData, col.mData, _MM_SHUFFLE(0, 0, 0, 0));
			switch(index) {
				case 1:
					t = _mm_shuffle_ps(mRows[0].mData, t, _MM_SHUFFLE(0, 0, 0, 0));
					mRows[0].mData = _mm_shuffle_ps(t, mRows[0].mData, _MM_SHUFFLE(3, 2, 2, 1));
					break;
				case 2:
					t = _mm_shuffle_ps(t, mRows[0].mData, _MM_SHUFFLE(3, 3, 0, 0));
					mRows[0].mData = _mm_shuffle_ps(mRows[0].mData, t, _MM_SHUFFLE(2, 1, 1, 0));
					break;
				case 3:
					t = _mm_shuffle_ps(mRows[0].mData, t, _MM_SHUFFLE(0, 0, 2, 2));
					mRows[0].mData = _mm_shuffle_ps(mRows[0].mData, t, _MM_SHUFFLE(2, 1, 1, 0));
					break;
			}
			// row 1
			t = _mm_shuffle_ps(col.mData, col.mData, _MM_SHUFFLE(1, 1, 1, 1));
			switch(index) {
				case 1:
					t = _mm_shuffle_ps(mRows[1].mData, t, _MM_SHUFFLE(0, 0, 0, 0));
					mRows[1].mData = _mm_shuffle_ps(t, mRows[1].mData, _MM_SHUFFLE(3, 2, 2, 1));
					break;
				case 2:
					t = _mm_shuffle_ps(t, mRows[1].mData, _MM_SHUFFLE(3, 3, 0, 0));
					mRows[1].mData = _mm_shuffle_ps(mRows[1].mData, t, _MM_SHUFFLE(2, 1, 1, 0));
					break;
				case 3:
					t = _mm_shuffle_ps(mRows[1].mData, t, _MM_SHUFFLE(0, 0, 2, 2));
					mRows[1].mData = _mm_shuffle_ps(mRows[1].mData, t, _MM_SHUFFLE(2, 1, 1, 0));
					break;
			}
			// row 2
			t = _mm_shuffle_ps(col.mData, col.mData, _MM_SHUFFLE(2, 2, 2, 2));
			switch(index) {
				case 1:
					t = _mm_shuffle_ps(mRows[2].mData, t, _MM_SHUFFLE(0, 0, 0, 0));
					mRows[2].mData = _mm_shuffle_ps(t, mRows[2].mData, _MM_SHUFFLE(3, 2, 2, 1));
					break;
				case 2:
					t = _mm_shuffle_ps(t, mRows[2].mData, _MM_SHUFFLE(3, 3, 0, 0));
					mRows[2].mData = _mm_shuffle_ps(mRows[2].mData, t, _MM_SHUFFLE(2, 1, 1, 0));
					break;
				case 3:
					t = _mm_shuffle_ps(mRows[2].mData, t, _MM_SHUFFLE(0, 0, 2, 2));
					mRows[2].mData = _mm_shuffle_ps(mRows[2].mData, t, _MM_SHUFFLE(2, 1, 1, 0));
					break;
			}
			// row 3
			t = _mm_shuffle_ps(col.mData, col.mData, _MM_SHUFFLE(3, 3, 3, 3));
			switch(index) {
				case 1:
					t = _mm_shuffle_ps(mRows[3].mData, t, _MM_SHUFFLE(0, 0, 0, 0));
					mRows[3].mData = _mm_shuffle_ps(t, mRows[3].mData, _MM_SHUFFLE(3, 2, 2, 1));
					break;
				case 2:
					t = _mm_shuffle_ps(t, mRows[3].mData, _MM_SHUFFLE(3, 3, 0, 0));
					mRows[3].mData = _mm_shuffle_ps(mRows[3].mData, t, _MM_SHUFFLE(2, 1, 1, 0));
					break;
				case 3:
					t = _mm_shuffle_ps(mRows[3].mData, t, _MM_SHUFFLE(0, 0, 2, 2));
					mRows[3].mData = _mm_shuffle_ps(mRows[3].mData, t, _MM_SHUFFLE(2, 1, 1, 0));
					break;
			}
#elif defined(SIMD_NONE)
			mRows[0].mData[index] = col.mData[0];
			mRows[1].mData[index] = col.mData[1];
			mRows[2].mData[index] = col.mData[2];
			mRows[3].mData[index] = col.mData[3];
#endif
		}
		void SetRow(int index, const VectorBase& row)
		{
			mRows[index] = row;
		}

		void Transpose()
		{
#if defined(SIMD_SSE)
			_MM_TRANSPOSE4_PS(mRows[0].mData, mRows[1].mData, mRows[2].mData, mRows[3].mData);
#elif defined(SIMD_NONE)
			float tmp;

			for(int i = 1; i < 4; i++) {
				for(int j = 0; j < i; j++) {
					tmp = mRows[i].mData[j];
					mRows[i].mData[j] = mRows[j].mData[i];
					mRows[j].mData[i] = tmp;
				}
			}
#endif
		}
		Matrix Transposed() const
		{
			Matrix res = *this;
			res.Transpose();

			return res;
		}

		void Inverse(); // TODO: 차후 구현
		Matrix Inversed() const; // TODO: 차후 구현

	private:
		VectorBase mRows[4];

		friend Matrix operator* (float lhs, const Matrix& rhs);

		friend VectorBase operator* (const Vector4& lhs, const Matrix& rhs);
	};

	inline Matrix operator* (float lhs, const Matrix& rhs)
	{
		Matrix r;
		r.mRows[0] = rhs.mRows[0] * lhs;
		r.mRows[1] = rhs.mRows[1] * lhs;
		r.mRows[2] = rhs.mRows[2] * lhs;
		r.mRows[3] = rhs.mRows[3] * lhs;

		return r;
	}

	inline VectorBase operator*(const Vector4& lhs, const Matrix& rhs)
	{
		VectorBase res;

#if defined(SIMD_SSE)
		VectorData v0, v1, v2, v3;

		v3 = lhs.mData;
		v0 = _mm_shuffle_ps(v3, v3, _MM_SHUFFLE(0, 0, 0, 0));
		v1 = _mm_shuffle_ps(v3, v3, _MM_SHUFFLE(1, 1, 1, 1));
		v2 = _mm_shuffle_ps(v3, v3, _MM_SHUFFLE(2, 2, 2, 2));
		v3 = _mm_shuffle_ps(v3, v3, _MM_SHUFFLE(3, 3, 3, 3));

		v0 = _mm_mul_ps(v0, rhs.mRows[0].mData);
		v1 = _mm_mul_ps(v1, rhs.mRows[1].mData);
		v2 = _mm_mul_ps(v2, rhs.mRows[2].mData);
		v3 = _mm_mul_ps(v3, rhs.mRows[3].mData);

		v2 = _mm_add_ps(v2, v3);
		v1 = _mm_add_ps(v1, v2);
		v0 = _mm_add_ps(v0, v1);
		res.mData = v0;

#elif defined(SIMD_NONE)
		float v0, v1, v2, v3;

		v0 = lhs.mData[0];
		v1 = lhs.mData[1];
		v2 = lhs.mData[2];
		v3 = lhs.mData[3];

		res.mData[0] = (v0*rhs.mRows[0].mData[0]) + (v1*rhs.mRows[1].mData[0]) + (v2*rhs.mRows[2].mData[0]) + (v3*rhs.mRows[3].mData[0]);
		res.mData[1] = (v0*rhs.mRows[0].mData[1]) + (v1*rhs.mRows[1].mData[1]) + (v2*rhs.mRows[2].mData[1]) + (v3*rhs.mRows[3].mData[1]);
		res.mData[2] = (v0*rhs.mRows[0].mData[2]) + (v1*rhs.mRows[1].mData[2]) + (v2*rhs.mRows[2].mData[2]) + (v3*rhs.mRows[3].mData[2]);
		res.mData[3] = (v0*rhs.mRows[0].mData[3]) + (v1*rhs.mRows[1].mData[3]) + (v2*rhs.mRows[2].mData[3]) + (v3*rhs.mRows[3].mData[3]);
#endif

		return res;
	}
} // namespace cube
