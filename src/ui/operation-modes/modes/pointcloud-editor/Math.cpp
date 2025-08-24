#include "Math.h"

namespace ui::operation_modes::modes::pointcloud_editor {
	using namespace ucsl::resources::pointcloud::v2;

	Eigen::Vector3f ToEuler(const Eigen::Matrix3f& rotation, RotationOrder rotationOrder) {
		switch (rotationOrder) {
		case RotationOrder::XYZ: {
			auto euler = rotation.eulerAngles(2, 1, 0);
			return { euler[2], euler[1], euler[0] };
		}
		case RotationOrder::YZX: {
			auto euler = rotation.eulerAngles(0, 2, 1);
			return { euler[0], euler[2], euler[1] };
		}
		case RotationOrder::ZXY: {
			auto euler = rotation.eulerAngles(1, 0, 2);
			return { euler[1], euler[0], euler[2] };
		}
		case RotationOrder::XZY: {
			auto euler = rotation.eulerAngles(1, 2, 0);
			return { euler[2], euler[0], euler[1] };
		}
		case RotationOrder::YXZ: {
			auto euler = rotation.eulerAngles(2, 0, 1);
			return { euler[1], euler[2], euler[0] };
		}
		case RotationOrder::ZYX: {
			auto euler = rotation.eulerAngles(0, 1, 2);
			return { euler[0], euler[1], euler[2] };
		}
		default: {
			auto euler = rotation.eulerAngles(2, 1, 0);
			return { euler[2], euler[1], euler[0] };
		}
		}
	}

	Eigen::Quaternionf FromEuler(const Eigen::Vector3f& rotation, RotationOrder rotationOrder) {
		switch (rotationOrder) {
		case RotationOrder::XYZ: return Eigen::AngleAxisf(rotation[2], Eigen::Vector3f::UnitZ()) * Eigen::AngleAxisf(rotation[1], Eigen::Vector3f::UnitY()) * Eigen::AngleAxisf(rotation[0], Eigen::Vector3f::UnitX());
		case RotationOrder::YZX: return Eigen::AngleAxisf(rotation[0], Eigen::Vector3f::UnitX()) * Eigen::AngleAxisf(rotation[2], Eigen::Vector3f::UnitZ()) * Eigen::AngleAxisf(rotation[1], Eigen::Vector3f::UnitY());
		case RotationOrder::ZXY: return Eigen::AngleAxisf(rotation[1], Eigen::Vector3f::UnitY()) * Eigen::AngleAxisf(rotation[0], Eigen::Vector3f::UnitX()) * Eigen::AngleAxisf(rotation[2], Eigen::Vector3f::UnitZ());
		case RotationOrder::XZY: return Eigen::AngleAxisf(rotation[1], Eigen::Vector3f::UnitY()) * Eigen::AngleAxisf(rotation[2], Eigen::Vector3f::UnitZ()) * Eigen::AngleAxisf(rotation[0], Eigen::Vector3f::UnitX());
		case RotationOrder::YXZ: return Eigen::AngleAxisf(rotation[2], Eigen::Vector3f::UnitZ()) * Eigen::AngleAxisf(rotation[0], Eigen::Vector3f::UnitX()) * Eigen::AngleAxisf(rotation[1], Eigen::Vector3f::UnitY());
		case RotationOrder::ZYX: return Eigen::AngleAxisf(rotation[0], Eigen::Vector3f::UnitX()) * Eigen::AngleAxisf(rotation[1], Eigen::Vector3f::UnitY()) * Eigen::AngleAxisf(rotation[2], Eigen::Vector3f::UnitZ());
		default: return Eigen::AngleAxisf(rotation[0], Eigen::Vector3f::UnitX()) * Eigen::AngleAxisf(rotation[1], Eigen::Vector3f::UnitY()) * Eigen::AngleAxisf(rotation[2], Eigen::Vector3f::UnitZ());
		}
	}

	void UpdateInstanceTransform(InstanceData& instance, const Eigen::Affine3f& transform) {
		Eigen::Matrix3f rotation;
		Eigen::Matrix3f scaling;

		transform.computeRotationScaling(&rotation, &scaling);

		instance.position = transform.translation();
		instance.rotation = ToEuler(rotation, instance.rotationOrder);
		instance.scale = scaling.diagonal();
	}

	Eigen::Affine3f GetInstanceTransform(const InstanceData& instance) {
		Eigen::Affine3f affine;
		affine.fromPositionOrientationScale(instance.position, FromEuler(instance.rotation, instance.rotationOrder), instance.scale);
		return affine;
	}
}
