#pragma once

namespace cube
{
	struct Vector2;
	struct Vector3;

	struct Vector2
	{
		float x;
		float y;

		static float Dot(const Vector2& lhs, const Vector2& rhs)
		{
			return lhs.x * rhs.x + lhs.y * rhs.y;
		}

		// Operators between vector2s
		Vector2& operator=(const Vector2& rhs)
		{
			if(this != &rhs) {
				this->x = rhs.x;
				this->y = rhs.y;
			}

			return *this;
		}

		Vector2 operator+(const Vector2& rhs)
		{
			Vector2 n;
			n.x = this->x + rhs.x;
			n.y = this->y + rhs.y;

			return n;
		}
		Vector2& operator+=(const Vector2& rhs)
		{
			this->x += rhs.x;
			this->y += rhs.y;

			return *this;
		}
		Vector2 operator-(const Vector2& rhs)
		{
			Vector2 n;
			n.x = this->x - rhs.x;
			n.y = this->y - rhs.y;

			return n;
		}
		Vector2& operator-=(const Vector2& rhs)
		{
			this->x -= rhs.x;
			this->y -= rhs.y;

			return *this;
		}

		Vector2 operator*(const float rhs)
		{
			Vector2 n;
			n.x = this->x * rhs;
			n.y = this->y * rhs;

			return n;
		}
		Vector2& operator*=(const float rhs)
		{
			this->x *= rhs;
			this->y *= rhs;

			return *this;
		}
		Vector2 operator/(const float rhs)
		{
			Vector2 n;
			n.x = this->x / rhs;
			n.y = this->y / rhs;

			return n;
		}
		Vector2& operator/=(const float rhs)
		{
			this->x /= rhs;
			this->y /= rhs;

			return *this;
		}

		// Operators between vector2 and vector3
		Vector2 operator+(const Vector3& rhs);
		Vector2& operator+=(const Vector3& rhs);
		Vector2 operator-(const Vector3& rhs);
		Vector2& operator-=(const Vector3& rhs);
	};

	struct Vector3
	{
		float x;
		float y;
		float z;

		static float Dot(const Vector3& lhs, const Vector3& rhs)
		{
			return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
		}
		static Vector3 Cross(const Vector3& lhs, const Vector3& rhs)
		{
			Vector3 n;

			n.x = lhs.y*rhs.z - lhs.z*rhs.x;
			n.y = lhs.z*rhs.x - lhs.x*rhs.z;
			n.z = lhs.x*rhs.y - lhs.y*rhs.x;

			return n;
		}

		// Operators between vector3s
		Vector3& operator=(const Vector3& rhs)
		{
			if(this != &rhs) {
				this->x = rhs.x;
				this->y = rhs.y;
				this->z = rhs.z;
			}

			return *this;
		}

		Vector3 operator+(const Vector3& rhs)
		{
			Vector3 n;
			n.x = this->x + rhs.x;
			n.y = this->y + rhs.y;
			n.z = this->z += rhs.z;

			return n;
		}
		Vector3& operator+=(const Vector3& rhs)
		{
			this->x += rhs.x;
			this->y += rhs.y;
			this->z += rhs.z;

			return *this;
		}
		Vector3 operator-(const Vector3& rhs)
		{
			Vector3 n;
			n.x = this->x - rhs.x;
			n.y = this->y - rhs.y;
			n.z = this->z - rhs.z;

			return n;
		}
		Vector3& operator-=(const Vector3& rhs)
		{
			this->x -= rhs.x;
			this->y -= rhs.y;
			this->z -= rhs.z;

			return *this;
		}

		Vector3 operator*(const float rhs)
		{
			Vector3 n;
			n.x = this->x * rhs;
			n.y = this->y * rhs;
			n.z = this->z * rhs;

			return n;
		}
		Vector3& operator*=(const float rhs)
		{
			this->x *= rhs;
			this->y *= rhs;
			this->z *= rhs;

			return *this;
		}
		Vector3 operator/(const float rhs)
		{
			Vector3 n;
			n.x = this->x / rhs;
			n.y = this->y / rhs;
			n.z = this->z / rhs;

			return n;
		}
		Vector3& operator/=(const float rhs)
		{
			this->x /= rhs;
			this->y /= rhs;
			this->z /= rhs;

			return *this;
		}

		// Operators between vector3 and vector2
		Vector3 operator+(const Vector2& rhs)
		{
			Vector3 n;
			n.x = this->x + rhs.x;
			n.y = this->y + rhs.y;
			n.z = this->z;

			return n;
		}
		Vector3& operator+=(const Vector2& rhs)
		{
			this->x += rhs.x;
			this->y += rhs.y;

			return *this;
		}
		Vector3 operator-(const Vector2& rhs)
		{
			Vector3 n;
			n.x = this->x - rhs.x;
			n.y = this->y - rhs.y;
			n.z = this->z;

			return n;
		}
		Vector3& operator-=(const Vector2& rhs)
		{
			this->x -= rhs.x;
			this->y -= rhs.y;

			return *this;
		}
	};


	inline Vector2 Vector2::operator+(const Vector3& rhs)
	{
		Vector2 n;
		n.x = this->x + rhs.x;
		n.y = this->y + rhs.y;

		return n;
	}
	inline Vector2& Vector2::operator+=(const Vector3& rhs)
	{
		this->x += rhs.x;
		this->y += rhs.y;

		return *this;
	}
	inline Vector2 Vector2::operator-(const Vector3& rhs)
	{
		Vector2 n;
		n.x = this->x - rhs.x;
		n.y = this->y - rhs.y;

		return n;
	}
	inline Vector2& Vector2::operator-=(const Vector3& rhs)
	{
		this->x -= rhs.x;
		this->y -= rhs.y;

		return *this;
	}
}