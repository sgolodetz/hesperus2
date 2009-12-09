##################################
# CMakeLists.txt for engine/core #
##################################

###########################
# Specify the target name #
###########################

SET(targetname hesperus)

#############################
# Specify the project files #
#############################

##
SET(audio_sources
hesp/audio/SoundInstance.cpp
hesp/audio/SoundInstanceUpdater.cpp
hesp/audio/SoundListenerUpdater.cpp
hesp/audio/SoundSystem.cpp
)

SET(audio_headers
hesp/audio/SoundInstance.h
hesp/audio/SoundInstanceUpdater.h
hesp/audio/SoundListenerUpdater.h
hesp/audio/SoundSystem.h
)

##
SET(axes_sources
hesp/axes/NUVAxes.cpp
hesp/axes/VerticallyConstrainedNUVAxes.cpp
)

SET(axes_headers
hesp/axes/NUVAxes.h
hesp/axes/VerticallyConstrainedNUVAxes.h
)

##
SET(cameras_sources
hesp/cameras/FirstPersonCamera.cpp
hesp/cameras/FixedCamera.cpp
)

SET(cameras_headers
hesp/cameras/Camera.h
hesp/cameras/FirstPersonCamera.h
hesp/cameras/FixedCamera.h
)

##
SET(colours_sources
hesp/colours/Colour3d.cpp
)

SET(colours_headers
hesp/colours/Colour3d.h
)

##
SET(datastructures_sources
hesp/datastructures/FiniteStateMachine.cpp
hesp/datastructures/FSMState.cpp
hesp/datastructures/FSMTransition.cpp
)

SET(datastructures_headers
hesp/datastructures/FiniteStateMachine.h
hesp/datastructures/FSMState.h
hesp/datastructures/FSMTransition.h
hesp/datastructures/PriorityQueue.h
)

SET(datastructures_templates
hesp/datastructures/PriorityQueue.tpp
)

##
SET(exceptions_headers
hesp/exceptions/Exception.h
hesp/exceptions/FileNotFoundException.h
hesp/exceptions/InvalidParameterException.h
)

##
SET(gui_sources
hesp/gui/Button.cpp
hesp/gui/ExplicitLayout.cpp
hesp/gui/GUIComponent.cpp
hesp/gui/LaidOutGUIComponent.cpp
hesp/gui/Picture.cpp
hesp/gui/Screen.cpp
)

SET(gui_headers
hesp/gui/Button.h
hesp/gui/ExplicitLayout.h
hesp/gui/Extents.h
hesp/gui/GUIComponent.h
hesp/gui/GUIContainer.h
hesp/gui/LaidOutGUIComponent.h
hesp/gui/Picture.h
hesp/gui/Screen.h
)

SET(gui_templates
hesp/gui/GUIContainer.tpp
)

##
SET(images_sources
hesp/images/BitmapLoader.cpp
hesp/images/BitmapSaver.cpp
hesp/images/ImageLoader.cpp
hesp/images/PNGLoader.cpp
hesp/images/PNGSaver.cpp
)

SET(images_headers
hesp/images/BitmapLoader.h
hesp/images/BitmapSaver.h
hesp/images/Image.h
hesp/images/ImageLoader.h
hesp/images/PixelTypes.h
hesp/images/PNGLoader.h
hesp/images/PNGSaver.h
hesp/images/SimpleImage.h
)

SET(images_templates
hesp/images/SimpleImage.tpp
)

##
SET(input_sources
hesp/input/InputAction.cpp
hesp/input/InputBinding.cpp
hesp/input/InputState.cpp
hesp/input/KeyInputter.cpp
hesp/input/MouseButtonInputter.cpp
)

SET(input_headers
hesp/input/InputAction.h
hesp/input/InputBinding.h
hesp/input/InputState.h
hesp/input/Inputter.h
hesp/input/KeyInputter.h
hesp/input/MouseButton.h
hesp/input/MouseButtonInputter.h
)

##
SET(io_files_sources
hesp/io/files/BindingFile.cpp
hesp/io/files/DefinitionsFile.cpp
hesp/io/files/DefinitionsSpecifierFile.cpp
hesp/io/files/LevelFile.cpp
hesp/io/files/LightsFile.cpp
hesp/io/files/ModelFiles.cpp
hesp/io/files/NavFile.cpp
hesp/io/files/ObjectsFile.cpp
hesp/io/files/OnionPortalsFile.cpp
hesp/io/files/PortalsFile.cpp
hesp/io/files/VisFile.cpp
)

SET(io_files_headers
hesp/io/files/BindingFile.h
hesp/io/files/BrushesFile.h
hesp/io/files/DefinitionsFile.h
hesp/io/files/DefinitionsSpecifierFile.h
hesp/io/files/GeometryFile.h
hesp/io/files/LevelFile.h
hesp/io/files/LightsFile.h
hesp/io/files/LitTreeFile.h
hesp/io/files/ModelFiles.h
hesp/io/files/NavFile.h
hesp/io/files/ObjectsFile.h
hesp/io/files/OnionPortalsFile.h
hesp/io/files/OnionTreeFile.h
hesp/io/files/PortalsFile.h
hesp/io/files/TreeFile.h
hesp/io/files/VisFile.h
)

SET(io_files_templates
hesp/io/files/BrushesFile.tpp
hesp/io/files/GeometryFile.tpp
hesp/io/files/LitTreeFile.tpp
hesp/io/files/OnionTreeFile.tpp
hesp/io/files/TreeFile.tpp
)

##
SET(io_sections_sources
hesp/io/sections/DefinitionsSpecifierSection.cpp
hesp/io/sections/LightmapPrefixSection.cpp
hesp/io/sections/LightmapsSection.cpp
hesp/io/sections/LightsSection.cpp
hesp/io/sections/ModelNamesSection.cpp
hesp/io/sections/NavSection.cpp
hesp/io/sections/ObjectsSection.cpp
hesp/io/sections/OnionTreeSection.cpp
hesp/io/sections/SpriteNamesSection.cpp
hesp/io/sections/TreeSection.cpp
hesp/io/sections/VisSection.cpp
)

SET(io_sections_headers
hesp/io/sections/DefinitionsSpecifierSection.h
hesp/io/sections/LightmapPrefixSection.h
hesp/io/sections/LightmapsSection.h
hesp/io/sections/LightsSection.h
hesp/io/sections/ModelNamesSection.h
hesp/io/sections/NavSection.h
hesp/io/sections/ObjectsSection.h
hesp/io/sections/OnionTreeSection.h
hesp/io/sections/PolygonsSection.h
hesp/io/sections/ResourceNamesSection.h
hesp/io/sections/SpriteNamesSection.h
hesp/io/sections/TreeSection.h
hesp/io/sections/VisSection.h
)

SET(io_sections_templates
hesp/io/sections/PolygonsSection.tpp
hesp/io/sections/ResourceNamesSection.tpp
)

##
SET(io_util_sources
hesp/io/util/DirectoryFinder.cpp
hesp/io/util/FieldIO.cpp
hesp/io/util/LineIO.cpp
hesp/io/util/NavLinkFactory.cpp
hesp/io/util/PropertyIO.cpp
hesp/io/util/PropFormatter.cpp
hesp/io/util/PropReader.cpp
)

SET(io_util_headers
hesp/io/util/DirectoryFinder.h
hesp/io/util/FieldIO.h
hesp/io/util/IOUtil.h
hesp/io/util/LineIO.h
hesp/io/util/NavLinkFactory.h
hesp/io/util/PropertyIO.h
hesp/io/util/PropFormatter.h
hesp/io/util/PropReader.h
)

SET(io_util_templates
hesp/io/util/FieldIO.tpp
hesp/io/util/IOUtil.tpp
hesp/io/util/PropertyIO.tpp
hesp/io/util/PropFormatter.tpp
hesp/io/util/PropReader.tpp
)

##
SET(level_sources
hesp/level/GeometryRenderer.cpp
hesp/level/HUDViewer.cpp
hesp/level/Level.cpp
hesp/level/LevelViewer.cpp
hesp/level/LitGeometryRenderer.cpp
hesp/level/UnlitGeometryRenderer.cpp
)

SET(level_headers
hesp/level/GeometryRenderer.h
hesp/level/HUDViewer.h
hesp/level/Level.h
hesp/level/LevelViewer.h
hesp/level/LitGeometryRenderer.h
hesp/level/UnlitGeometryRenderer.h
)

##
SET(level_bounds_sources
hesp/level/bounds/AABBBounds.cpp
hesp/level/bounds/BoundsManager.cpp
hesp/level/bounds/PointBounds.cpp
hesp/level/bounds/SphereBounds.cpp
)

SET(level_bounds_headers
hesp/level/bounds/AABBBounds.h
hesp/level/bounds/Bounds.h
hesp/level/bounds/BoundsManager.h
hesp/level/bounds/PointBounds.h
hesp/level/bounds/SphereBounds.h
)

##
SET(level_brushes_sources
hesp/level/brushes/BrushExpander.cpp
hesp/level/brushes/BrushFunction.cpp
)

SET(level_brushes_headers
hesp/level/brushes/BrushExpander.h
hesp/level/brushes/BrushFunction.h
hesp/level/brushes/PolyhedralBrush.h
)

SET(level_brushes_templates
hesp/level/brushes/PolyhedralBrush.tpp
)

##
SET(level_csg_headers
hesp/level/csg/CSGUtil.h
)

SET(level_csg_templates
hesp/level/csg/CSGUtil.tpp
)

##
SET(level_lighting_sources
hesp/level/lighting/Lightmap.cpp
hesp/level/lighting/LightmapGenerator.cpp
hesp/level/lighting/LightmapGrid.cpp
)

SET(level_lighting_headers
hesp/level/lighting/Light.h
hesp/level/lighting/Lightmap.h
hesp/level/lighting/LightmapGenerator.h
hesp/level/lighting/LightmapGrid.h
)

SET(level_lighting_templates
hesp/level/lighting/LightmapGrid.tpp
)

##
SET(level_models_sources
hesp/level/models/Animation.cpp
hesp/level/models/AnimationController.cpp
hesp/level/models/Bone.cpp
hesp/level/models/BoneHierarchy.cpp
hesp/level/models/BoneWeight.cpp
hesp/level/models/ConfiguredBone.cpp
hesp/level/models/ConfiguredPose.cpp
hesp/level/models/Mesh.cpp
hesp/level/models/Model.cpp
hesp/level/models/ModelManager.cpp
hesp/level/models/ModelVertex.cpp
hesp/level/models/Pose.cpp
hesp/level/models/Skeleton.cpp
hesp/level/models/Submesh.cpp
)

SET(level_models_headers
hesp/level/models/Animation.h
hesp/level/models/AnimationController.h
hesp/level/models/Bone.h
hesp/level/models/BoneHierarchy.h
hesp/level/models/BoneWeight.h
hesp/level/models/ConfiguredBone.h
hesp/level/models/ConfiguredPose.h
hesp/level/models/Mesh.h
hesp/level/models/Model.h
hesp/level/models/ModelManager.h
hesp/level/models/ModelVertex.h
hesp/level/models/Pose.h
hesp/level/models/PoseModifier.h
hesp/level/models/Skeleton.h
hesp/level/models/Submesh.h
)

##
SET(level_nav_sources
hesp/level/nav/AdjacencyList.cpp
hesp/level/nav/AdjacencyTable.cpp
hesp/level/nav/GlobalPathfinder.cpp
hesp/level/nav/NavManager.cpp
hesp/level/nav/NavMesh.cpp
hesp/level/nav/NavMeshGenerator.cpp
hesp/level/nav/NavMeshUtil.cpp
hesp/level/nav/NavPolygon.cpp
hesp/level/nav/PathTable.cpp
hesp/level/nav/PathTableGenerator.cpp
hesp/level/nav/StepDownLink.cpp
hesp/level/nav/StepLink.cpp
hesp/level/nav/StepUpLink.cpp
hesp/level/nav/WalkLink.cpp
)

SET(level_nav_headers
hesp/level/nav/AdjacencyList.h
hesp/level/nav/AdjacencyTable.h
hesp/level/nav/GlobalPathfinder.h
hesp/level/nav/NavDataset.h
hesp/level/nav/NavLink.h
hesp/level/nav/NavManager.h
hesp/level/nav/NavMesh.h
hesp/level/nav/NavMeshGenerator.h
hesp/level/nav/NavMeshUtil.h
hesp/level/nav/NavPolygon.h
hesp/level/nav/PathTable.h
hesp/level/nav/PathTableGenerator.h
hesp/level/nav/StepDownLink.h
hesp/level/nav/StepLink.h
hesp/level/nav/StepUpLink.h
hesp/level/nav/WalkLink.h
)

##
SET(level_objects_base_sources
hesp/level/objects/base/ComponentPropertyTypeMap.cpp
hesp/level/objects/base/IObjectComponent.cpp
hesp/level/objects/base/ListenerTable.cpp
hesp/level/objects/base/ObjectID.cpp
hesp/level/objects/base/ObjectManager.cpp
hesp/level/objects/base/ObjectSpecification.cpp
)

SET(level_objects_base_headers
hesp/level/objects/base/ComponentPropertyTypeMap.h
hesp/level/objects/base/IObjectComponent.h
hesp/level/objects/base/IYoke.h
hesp/level/objects/base/ListenerTable.h
hesp/level/objects/base/Message.h
hesp/level/objects/base/MessageHandler.h
hesp/level/objects/base/ObjectCommand.h
hesp/level/objects/base/ObjectComponent.h
hesp/level/objects/base/ObjectID.h
hesp/level/objects/base/ObjectManager.h
hesp/level/objects/base/ObjectSpecification.h
)

SET(level_objects_base_templates
hesp/level/objects/base/ObjectComponent.tpp
hesp/level/objects/base/ObjectManager.tpp
hesp/level/objects/base/ObjectSpecification.tpp
)

##
SET(level_objects_commands_sources
hesp/level/objects/commands/CmdBipedChangePosture.cpp
hesp/level/objects/commands/CmdBipedJump.cpp
hesp/level/objects/commands/CmdBipedMove.cpp
hesp/level/objects/commands/CmdBipedRun.cpp
hesp/level/objects/commands/CmdBipedSetLook.cpp
hesp/level/objects/commands/CmdBipedTurn.cpp
hesp/level/objects/commands/CmdBipedWalk.cpp
hesp/level/objects/commands/CmdUseActiveItem.cpp
)

SET(level_objects_commands_headers
hesp/level/objects/commands/CmdBipedChangePosture.h
hesp/level/objects/commands/CmdBipedJump.h
hesp/level/objects/commands/CmdBipedMove.h
hesp/level/objects/commands/CmdBipedRun.h
hesp/level/objects/commands/CmdBipedSetLook.h
hesp/level/objects/commands/CmdBipedTurn.h
hesp/level/objects/commands/CmdBipedWalk.h
hesp/level/objects/commands/CmdUseActiveItem.h
)

##
SET(level_objects_components_Activatable_sources
hesp/level/objects/components/CmpConsumeActivatable.cpp
hesp/level/objects/components/CmpItemActivatable.cpp
hesp/level/objects/components/ICmpActivatable.cpp
)

SET(level_objects_components_Activatable_headers
hesp/level/objects/components/CmpConsumeActivatable.h
hesp/level/objects/components/CmpItemActivatable.h
hesp/level/objects/components/ICmpActivatable.h
)

SET(level_objects_components_AnimChooser_sources
hesp/level/objects/components/CmpBipedAnimChooser.cpp
)

SET(level_objects_components_AnimChooser_headers
hesp/level/objects/components/CmpBipedAnimChooser.h
hesp/level/objects/components/ICmpAnimChooser.h
hesp/level/objects/components/ICmpBipedAnimChooser.h
)

SET(level_objects_components_Health_sources
hesp/level/objects/components/CmpHealth.cpp
)

SET(level_objects_components_Health_headers
hesp/level/objects/components/CmpHealth.h
hesp/level/objects/components/ICmpHealth.h
)

SET(level_objects_components_Inventory_sources
hesp/level/objects/components/CmpInventory.cpp
)

SET(level_objects_components_Inventory_headers
hesp/level/objects/components/CmpInventory.h
hesp/level/objects/components/ICmpInventory.h
)

SET(level_objects_components_Movement_sources
hesp/level/objects/components/CmpMovement.cpp
)

SET(level_objects_components_Movement_headers
hesp/level/objects/components/CmpMovement.h
hesp/level/objects/components/ICmpMovement.h
)

SET(level_objects_components_Orientation_sources
hesp/level/objects/components/CmpOrientation.cpp
)

SET(level_objects_components_Orientation_headers
hesp/level/objects/components/CmpOrientation.h
hesp/level/objects/components/ICmpOrientation.h
)

SET(level_objects_components_Ownable_sources
hesp/level/objects/components/CmpOwnable.cpp
)

SET(level_objects_components_Ownable_headers
hesp/level/objects/components/CmpOwnable.h
hesp/level/objects/components/ICmpOwnable.h
)

SET(level_objects_components_Projectile_sources
hesp/level/objects/components/CmpBasicProjectile.cpp
)

SET(level_objects_components_Projectile_headers
hesp/level/objects/components/CmpBasicProjectile.h
hesp/level/objects/components/ICmpProjectile.h
)

SET(level_objects_components_Render_sources
hesp/level/objects/components/CmpBasicModelRender.cpp
hesp/level/objects/components/CmpCharacterModelRender.cpp
hesp/level/objects/components/CmpModelRender.cpp
hesp/level/objects/components/CmpSpriteRender.cpp
)

SET(level_objects_components_Render_headers
hesp/level/objects/components/CmpBasicModelRender.h
hesp/level/objects/components/CmpCharacterModelRender.h
hesp/level/objects/components/CmpModelRender.h
hesp/level/objects/components/CmpSpriteRender.h
hesp/level/objects/components/ICmpBasicModelRender.h
hesp/level/objects/components/ICmpModelRender.h
hesp/level/objects/components/ICmpRender.h
)

SET(level_objects_components_Simulation_sources
hesp/level/objects/components/CmpPosition.cpp
hesp/level/objects/components/CmpSimulation.cpp
)

SET(level_objects_components_Simulation_headers
hesp/level/objects/components/CmpPosition.h
hesp/level/objects/components/CmpSimulation.h
hesp/level/objects/components/ICmpPosition.h
hesp/level/objects/components/ICmpSimulation.h
)

SET(level_objects_components_Usable_sources
hesp/level/objects/components/CmpProjectileWeaponUsable.cpp
hesp/level/objects/components/CmpScriptedUsable.cpp
hesp/level/objects/components/CmpUsable.cpp
)

SET(level_objects_components_Usable_headers
hesp/level/objects/components/CmpProjectileWeaponUsable.h
hesp/level/objects/components/CmpScriptedUsable.h
hesp/level/objects/components/CmpUsable.h
hesp/level/objects/components/ICmpAmmoNeedingUsable.h
hesp/level/objects/components/ICmpUsable.h
)

SET(level_objects_components_Yoke_sources
hesp/level/objects/components/CmpMinimusScriptYoke.cpp
hesp/level/objects/components/CmpUserBipedYoke.cpp
)

SET(level_objects_components_Yoke_headers
hesp/level/objects/components/CmpMinimusScriptYoke.h
hesp/level/objects/components/CmpUserBipedYoke.h
hesp/level/objects/components/ICmpYoke.h
)

##
SET(level_objects_contactresolvers_sources
hesp/level/objects/contactresolvers/AbsorbProjectileContactResolver.cpp
hesp/level/objects/contactresolvers/BounceContactResolver.cpp
hesp/level/objects/contactresolvers/ProjectileDamageContactResolver.cpp
)

SET(level_objects_contactresolvers_headers
hesp/level/objects/contactresolvers/AbsorbProjectileContactResolver.h
hesp/level/objects/contactresolvers/BounceContactResolver.h
hesp/level/objects/contactresolvers/ProjectileDamageContactResolver.h
)

##
SET(level_objects_forcegenerators_sources
hesp/level/objects/forcegenerators/WeightForceGenerator.cpp
)

SET(level_objects_forcegenerators_headers
hesp/level/objects/forcegenerators/WeightForceGenerator.h
)

##
SET(level_objects_messages_sources
hesp/level/objects/messages/MsgObjectDestroyed.cpp
hesp/level/objects/messages/MsgObjectPredestroyed.cpp
hesp/level/objects/messages/MsgTimeElapsed.cpp
)

SET(level_objects_messages_headers
hesp/level/objects/messages/MsgObjectDestroyed.h
hesp/level/objects/messages/MsgObjectPredestroyed.h
hesp/level/objects/messages/MsgTimeElapsed.h
)

##
SET(level_objects_yokes_minimus_sources
hesp/level/objects/yokes/minimus/MinimusGotoPositionYoke.cpp
hesp/level/objects/yokes/minimus/MinimusScriptYoke.cpp
)

SET(level_objects_yokes_minimus_headers
hesp/level/objects/yokes/minimus/MinimusGotoPositionYoke.h
hesp/level/objects/yokes/minimus/MinimusScriptYoke.h
)

##
SET(level_objects_yokes_user_sources
hesp/level/objects/yokes/user/UserBipedYoke.cpp
)

SET(level_objects_yokes_user_headers
hesp/level/objects/yokes/user/UserBipedYoke.h
)

##
SET(level_physics_sources
hesp/level/physics/BoundsSupportMapping.cpp
hesp/level/physics/BroadPhaseCollisionDetector.cpp
hesp/level/physics/Contact.cpp
hesp/level/physics/ContactResolver.cpp
hesp/level/physics/ContactResolverRegistry.cpp
hesp/level/physics/ForceGeneratorRegistry.cpp
hesp/level/physics/MinkDiffSupportMapping.cpp
hesp/level/physics/NarrowPhaseCollisionDetector.cpp
hesp/level/physics/NormalPhysicsObject.cpp
hesp/level/physics/PhysicsObject.cpp
hesp/level/physics/PhysicsSystem.cpp
hesp/level/physics/SegmentSupportMapping.cpp
hesp/level/physics/SweptSupportMapping.cpp
hesp/level/physics/TranslatedSupportMapping.cpp
)

SET(level_physics_headers
hesp/level/physics/BoundsSupportMapping.h
hesp/level/physics/BroadPhaseCollisionDetector.h
hesp/level/physics/Contact.h
hesp/level/physics/ContactResolver.h
hesp/level/physics/ContactResolverRegistry.h
hesp/level/physics/ForceGenerator.h
hesp/level/physics/ForceGeneratorRegistry.h
hesp/level/physics/MinkDiffSupportMapping.h
hesp/level/physics/NarrowPhaseCollisionDetector.h
hesp/level/physics/NormalPhysicsObject.h
hesp/level/physics/PhysicsMaterial.h
hesp/level/physics/PhysicsObject.h
hesp/level/physics/PhysicsSystem.h
hesp/level/physics/SegmentSupportMapping.h
hesp/level/physics/SupportMapping.h
hesp/level/physics/SweptSupportMapping.h
hesp/level/physics/TranslatedSupportMapping.h
)

##
SET(level_portals_sources
hesp/level/portals/OnionPortal.cpp
hesp/level/portals/OnionPortalGenerator.cpp
hesp/level/portals/Portal.cpp
hesp/level/portals/PortalGenerator.cpp
)

SET(level_portals_headers
hesp/level/portals/BasePortalGenerator.h
hesp/level/portals/OnionPortal.h
hesp/level/portals/OnionPortalGenerator.h
hesp/level/portals/Portal.h
hesp/level/portals/PortalGenerator.h
)

SET(level_portals_templates
hesp/level/portals/BasePortalGenerator.tpp
)

##
SET(level_sprites_sources
hesp/level/sprites/Sprite.cpp
hesp/level/sprites/SpriteManager.cpp
)

SET(level_sprites_headers
hesp/level/sprites/Sprite.h
hesp/level/sprites/SpriteManager.h
)

##
SET(level_trees_sources
hesp/level/trees/BSPBranch.cpp
hesp/level/trees/BSPLeaf.cpp
hesp/level/trees/BSPNode.cpp
hesp/level/trees/BSPTree.cpp
hesp/level/trees/BSPUtil.cpp
hesp/level/trees/OnionBranch.cpp
hesp/level/trees/OnionLeaf.cpp
hesp/level/trees/OnionNode.cpp
hesp/level/trees/OnionTree.cpp
hesp/level/trees/OnionUtil.cpp
)

SET(level_trees_headers
hesp/level/trees/BSPBranch.h
hesp/level/trees/BSPCompiler.h
hesp/level/trees/BSPLeaf.h
hesp/level/trees/BSPNode.h
hesp/level/trees/BSPTree.h
hesp/level/trees/BSPUtil.h
hesp/level/trees/OnionBranch.h
hesp/level/trees/OnionCompiler.h
hesp/level/trees/OnionLeaf.h
hesp/level/trees/OnionNode.h
hesp/level/trees/OnionTree.h
hesp/level/trees/OnionUtil.h
hesp/level/trees/TreeUtil.h
)

SET(level_trees_templates
hesp/level/trees/BSPCompiler.tpp
hesp/level/trees/BSPUtil.tpp
hesp/level/trees/OnionCompiler.tpp
hesp/level/trees/TreeUtil.tpp
)

##
SET(level_vis_sources
hesp/level/vis/Antipenumbra.cpp
hesp/level/vis/VisCalculator.cpp
)

SET(level_vis_headers
hesp/level/vis/Antipenumbra.h
hesp/level/vis/VisCalculator.h
hesp/level/vis/VisTable.h
)

SET(level_vis_templates
hesp/level/vis/Antipenumbra.tpp
hesp/level/vis/VisTable.tpp
)

##
SET(materials_sources
hesp/materials/BasicMaterial.cpp
hesp/materials/TextureMaterial.cpp
)

SET(materials_headers
hesp/materials/BasicMaterial.h
hesp/materials/Material.h
hesp/materials/TextureMaterial.h
)

##
SET(math_sources
hesp/math/Constants.cpp
hesp/math/Interval.cpp
hesp/math/MathUtil.cpp
)

SET(math_headers
hesp/math/Constants.h
hesp/math/Interval.h
hesp/math/MathUtil.h
)

##
SET(math_geom_sources
hesp/math/geom/AABB.cpp
hesp/math/geom/GeomUtil.cpp
hesp/math/geom/Plane.cpp
hesp/math/geom/Sphere.cpp
hesp/math/geom/UniquePlanePred.cpp
)

SET(math_geom_headers
hesp/math/geom/AABB.h
hesp/math/geom/GeomUtil.h
hesp/math/geom/LineSegment.h
hesp/math/geom/Plane.h
hesp/math/geom/Polygon.h
hesp/math/geom/Sphere.h
hesp/math/geom/UniquePlanePred.h
)

SET(math_geom_templates
hesp/math/geom/GeomUtil.tpp
hesp/math/geom/Polygon.tpp
)

##
SET(math_matrices_sources
hesp/math/matrices/RBTMatrix.cpp
)

SET(math_matrices_headers
hesp/math/matrices/RBTMatrix.h
)

##
SET(math_quaternions_sources
hesp/math/quaternions/Quaternion.cpp
hesp/math/quaternions/RBTQuaternion.cpp
)

SET(math_quaternions_headers
hesp/math/quaternions/Quaternion.h
hesp/math/quaternions/RBTQuaternion.h
)

##
SET(math_vectors_sources
hesp/math/vectors/TexturedLitVector3d.cpp
hesp/math/vectors/TexturedVector3d.cpp
hesp/math/vectors/Vector2d.cpp
hesp/math/vectors/Vector3.cpp
hesp/math/vectors/VectorUtil.cpp
)

SET(math_vectors_headers
hesp/math/vectors/TexCoords.h
hesp/math/vectors/TexturedLitVector3d.h
hesp/math/vectors/TexturedVector3d.h
hesp/math/vectors/Vector2d.h
hesp/math/vectors/Vector3.h
hesp/math/vectors/VectorUtil.h
)

##
SET(ogl_headers
hesp/ogl/WrappedGL.h
)

##
SET(textures_sources
hesp/textures/Image24Texture.cpp
hesp/textures/Image32Texture.cpp
hesp/textures/Texture.cpp
hesp/textures/TextureFactory.cpp
)

SET(textures_headers
hesp/textures/Image24Texture.h
hesp/textures/Image32Texture.h
hesp/textures/Texture.h
hesp/textures/TextureFactory.h
)

##
SET(util_sources
hesp/util/ConfigOptions.cpp
hesp/util/IDAllocator.cpp
hesp/util/PolygonTypes.cpp
hesp/util/Properties.cpp
hesp/util/TextRenderer.cpp
)

SET(util_headers
hesp/util/ConfigOptions.h
hesp/util/IDAllocator.h
hesp/util/PolygonTypes.h
hesp/util/Properties.h
hesp/util/ResourceManager.h
hesp/util/TextRenderer.h
)

SET(util_templates
hesp/util/ConfigOptions.tpp
hesp/util/Properties.tpp
hesp/util/ResourceManager.tpp
)

##
SET(xml_sources
hesp/xml/XMLElement.cpp
hesp/xml/XMLLexer.cpp
hesp/xml/XMLParser.cpp
)

SET(xml_headers
hesp/xml/XMLElement.h
hesp/xml/XMLLexer.h
hesp/xml/XMLParser.h
hesp/xml/XMLToken.h
)

#################################################################
# Collect the project files into sources, headers and templates #
#################################################################

SET(sources
${audio_sources}
${axes_sources}
${cameras_sources}
${colours_sources}
${datastructures_sources}
${gui_sources}
${images_sources}
${input_sources}
${io_files_sources}
${io_sections_sources}
${io_util_sources}
${level_sources}
${level_bounds_sources}
${level_brushes_sources}
${level_lighting_sources}
${level_models_sources}
${level_nav_sources}
${level_objects_base_sources}
${level_objects_commands_sources}
${level_objects_components_Activatable_sources}
${level_objects_components_AnimChooser_sources}
${level_objects_components_Health_sources}
${level_objects_components_Inventory_sources}
${level_objects_components_Movement_sources}
${level_objects_components_Orientation_sources}
${level_objects_components_Ownable_sources}
${level_objects_components_Projectile_sources}
${level_objects_components_Render_sources}
${level_objects_components_Simulation_sources}
${level_objects_components_Usable_sources}
${level_objects_components_Yoke_sources}
${level_objects_contactresolvers_sources}
${level_objects_forcegenerators_sources}
${level_objects_messages_sources}
${level_objects_yokes_minimus_sources}
${level_objects_yokes_user_sources}
${level_physics_sources}
${level_portals_sources}
${level_sprites_sources}
${level_trees_sources}
${level_vis_sources}
${materials_sources}
${math_sources}
${math_geom_sources}
${math_matrices_sources}
${math_quaternions_sources}
${math_vectors_sources}
${textures_sources}
${util_sources}
${xml_sources}
)

SET(headers
${audio_headers}
${axes_headers}
${cameras_headers}
${colours_headers}
${datastructures_headers}
${exceptions_headers}
${gui_headers}
${images_headers}
${input_headers}
${io_files_headers}
${io_sections_headers}
${io_util_headers}
${level_headers}
${level_bounds_headers}
${level_brushes_headers}
${level_csg_headers}
${level_lighting_headers}
${level_models_headers}
${level_nav_headers}
${level_objects_base_headers}
${level_objects_commands_headers}
${level_objects_components_Activatable_headers}
${level_objects_components_AnimChooser_headers}
${level_objects_components_Health_headers}
${level_objects_components_Inventory_headers}
${level_objects_components_Movement_headers}
${level_objects_components_Orientation_headers}
${level_objects_components_Ownable_headers}
${level_objects_components_Projectile_headers}
${level_objects_components_Render_headers}
${level_objects_components_Simulation_headers}
${level_objects_components_Usable_headers}
${level_objects_components_Yoke_headers}
${level_objects_contactresolvers_headers}
${level_objects_forcegenerators_headers}
${level_objects_messages_headers}
${level_objects_yokes_minimus_headers}
${level_objects_yokes_user_headers}
${level_physics_headers}
${level_portals_headers}
${level_sprites_headers}
${level_trees_headers}
${level_vis_headers}
${materials_headers}
${math_headers}
${math_geom_headers}
${math_matrices_headers}
${math_quaternions_headers}
${math_vectors_headers}
${ogl_headers}
${textures_headers}
${util_headers}
${xml_headers}
)

SET(templates
${datastructures_templates}
${gui_templates}
${images_templates}
${io_files_templates}
${io_sections_templates}
${io_util_templates}
${level_brushes_templates}
${level_csg_templates}
${level_lighting_templates}
${level_objects_base_templates}
${level_portals_templates}
${level_trees_templates}
${level_vis_templates}
${math_geom_templates}
${util_templates}
)

#############################
# Specify the source groups #
#############################

##
SOURCE_GROUP(audio\\.cpp FILES ${audio_sources})
SOURCE_GROUP(audio\\.h FILES ${audio_headers})

##
SOURCE_GROUP(axes\\.cpp FILES ${axes_sources})
SOURCE_GROUP(axes\\.h FILES ${axes_headers})

##
SOURCE_GROUP(cameras\\.cpp FILES ${cameras_sources})
SOURCE_GROUP(cameras\\.h FILES ${cameras_headers})

##
SOURCE_GROUP(colours\\.cpp FILES ${colours_sources})
SOURCE_GROUP(colours\\.h FILES ${colours_headers})

##
SOURCE_GROUP(datastructures\\.cpp FILES ${datastructures_sources})
SOURCE_GROUP(datastructures\\.h FILES ${datastructures_headers})
SOURCE_GROUP(datastructures\\.tpp FILES ${datastructures_templates})

##
SOURCE_GROUP(exceptions\\.h FILES ${exceptions_headers})

##
SOURCE_GROUP(gui\\.cpp FILES ${gui_sources})
SOURCE_GROUP(gui\\.h FILES ${gui_headers})
SOURCE_GROUP(gui\\.tpp FILES ${gui_templates})

##
SOURCE_GROUP(images\\.cpp FILES ${images_sources})
SOURCE_GROUP(images\\.h FILES ${images_headers})
SOURCE_GROUP(images\\.tpp FILES ${images_templates})

##
SOURCE_GROUP(input\\.cpp FILES ${input_sources})
SOURCE_GROUP(input\\.h FILES ${input_headers})

##
SOURCE_GROUP(io\\files\\.cpp FILES ${io_files_sources})
SOURCE_GROUP(io\\files\\.h FILES ${io_files_headers})
SOURCE_GROUP(io\\files\\.tpp FILES ${io_files_templates})

##
SOURCE_GROUP(io\\sections\\.cpp FILES ${io_sections_sources})
SOURCE_GROUP(io\\sections\\.h FILES ${io_sections_headers})
SOURCE_GROUP(io\\sections\\.tpp FILES ${io_sections_templates})

##
SOURCE_GROUP(io\\util\\.cpp FILES ${io_util_sources})
SOURCE_GROUP(io\\util\\.h FILES ${io_util_headers})
SOURCE_GROUP(io\\util\\.tpp FILES ${io_util_templates})

##
SOURCE_GROUP(level\\.cpp FILES ${level_sources})
SOURCE_GROUP(level\\.h FILES ${level_headers})

##
SOURCE_GROUP(level\\bounds\\.cpp FILES ${level_bounds_sources})
SOURCE_GROUP(level\\bounds\\.h FILES ${level_bounds_headers})

##
SOURCE_GROUP(level\\brushes\\.cpp FILES ${level_brushes_sources})
SOURCE_GROUP(level\\brushes\\.h FILES ${level_brushes_headers})
SOURCE_GROUP(level\\brushes\\.tpp FILES ${level_brushes_templates})

##
SOURCE_GROUP(level\\csg\\.h FILES ${level_csg_headers})
SOURCE_GROUP(level\\csg\\.tpp FILES ${level_csg_templates})

##
SOURCE_GROUP(level\\lighting\\.cpp FILES ${level_lighting_sources})
SOURCE_GROUP(level\\lighting\\.h FILES ${level_lighting_headers})
SOURCE_GROUP(level\\lighting\\.tpp FILES ${level_lighting_templates})

##
SOURCE_GROUP(level\\models\\.cpp FILES ${level_models_sources})
SOURCE_GROUP(level\\models\\.h FILES ${level_models_headers})

##
SOURCE_GROUP(level\\nav\\.cpp FILES ${level_nav_sources})
SOURCE_GROUP(level\\nav\\.h FILES ${level_nav_headers})

##
SOURCE_GROUP(level\\objects\\base\\.cpp FILES ${level_objects_base_sources})
SOURCE_GROUP(level\\objects\\base\\.h FILES ${level_objects_base_headers})
SOURCE_GROUP(level\\objects\\base\\.tpp FILES ${level_objects_base_templates})

##
SOURCE_GROUP(level\\objects\\commands\\.cpp FILES ${level_objects_commands_sources})
SOURCE_GROUP(level\\objects\\commands\\.h FILES ${level_objects_commands_headers})

##
SOURCE_GROUP(level\\objects\\components\\Activatable\\.cpp FILES ${level_objects_components_Activatable_sources})
SOURCE_GROUP(level\\objects\\components\\Activatable\\.h FILES ${level_objects_components_Activatable_headers})
SOURCE_GROUP(level\\objects\\components\\AnimChooser\\.cpp FILES ${level_objects_components_AnimChooser_sources})
SOURCE_GROUP(level\\objects\\components\\AnimChooser\\.h FILES ${level_objects_components_AnimChooser_headers})
SOURCE_GROUP(level\\objects\\components\\Health\\.cpp FILES ${level_objects_components_Health_sources})
SOURCE_GROUP(level\\objects\\components\\Health\\.h FILES ${level_objects_components_Health_headers})
SOURCE_GROUP(level\\objects\\components\\Inventory\\.cpp FILES ${level_objects_components_Inventory_sources})
SOURCE_GROUP(level\\objects\\components\\Inventory\\.h FILES ${level_objects_components_Inventory_headers})
SOURCE_GROUP(level\\objects\\components\\Movement\\.cpp FILES ${level_objects_components_Movement_sources})
SOURCE_GROUP(level\\objects\\components\\Movement\\.h FILES ${level_objects_components_Movement_headers})
SOURCE_GROUP(level\\objects\\components\\Orientation\\.cpp FILES ${level_objects_components_Orientation_sources})
SOURCE_GROUP(level\\objects\\components\\Orientation\\.h FILES ${level_objects_components_Orientation_headers})
SOURCE_GROUP(level\\objects\\components\\Ownable\\.cpp FILES ${level_objects_components_Ownable_sources})
SOURCE_GROUP(level\\objects\\components\\Ownable\\.h FILES ${level_objects_components_Ownable_headers})
SOURCE_GROUP(level\\objects\\components\\Projectile\\.cpp FILES ${level_objects_components_Projectile_sources})
SOURCE_GROUP(level\\objects\\components\\Projectile\\.h FILES ${level_objects_components_Projectile_headers})
SOURCE_GROUP(level\\objects\\components\\Render\\.cpp FILES ${level_objects_components_Render_sources})
SOURCE_GROUP(level\\objects\\components\\Render\\.h FILES ${level_objects_components_Render_headers})
SOURCE_GROUP(level\\objects\\components\\Simulation\\.cpp FILES ${level_objects_components_Simulation_sources})
SOURCE_GROUP(level\\objects\\components\\Simulation\\.h FILES ${level_objects_components_Simulation_headers})
SOURCE_GROUP(level\\objects\\components\\Usable\\.cpp FILES ${level_objects_components_Usable_sources})
SOURCE_GROUP(level\\objects\\components\\Usable\\.h FILES ${level_objects_components_Usable_headers})
SOURCE_GROUP(level\\objects\\components\\Yoke\\.cpp FILES ${level_objects_components_Yoke_sources})
SOURCE_GROUP(level\\objects\\components\\Yoke\\.h FILES ${level_objects_components_Yoke_headers})

##
SOURCE_GROUP(level\\objects\\contactresolvers\\.cpp FILES ${level_objects_contactresolvers_sources})
SOURCE_GROUP(level\\objects\\contactresolvers\\.h FILES ${level_objects_contactresolvers_headers})

##
SOURCE_GROUP(level\\objects\\forcegenerators\\.cpp FILES ${level_objects_forcegenerators_sources})
SOURCE_GROUP(level\\objects\\forcegenerators\\.h FILES ${level_objects_forcegenerators_headers})

##
SOURCE_GROUP(level\\objects\\messages\\.cpp FILES ${level_objects_messages_sources})
SOURCE_GROUP(level\\objects\\messages\\.h FILES ${level_objects_messages_headers})

##
SOURCE_GROUP(level\\objects\\yokes\\minimus\\.cpp FILES ${level_objects_yokes_minimus_sources})
SOURCE_GROUP(level\\objects\\yokes\\minimus\\.h FILES ${level_objects_yokes_minimus_headers})

##
SOURCE_GROUP(level\\objects\\yokes\\user\\.cpp FILES ${level_objects_yokes_user_sources})
SOURCE_GROUP(level\\objects\\yokes\\user\\.h FILES ${level_objects_yokes_user_headers})

##
SOURCE_GROUP(level\\physics\\.cpp FILES ${level_physics_sources})
SOURCE_GROUP(level\\physics\\.h FILES ${level_physics_headers})

##
SOURCE_GROUP(level\\portals\\.cpp FILES ${level_portals_sources})
SOURCE_GROUP(level\\portals\\.h FILES ${level_portals_headers})
SOURCE_GROUP(level\\portals\\.tpp FILES ${level_portals_templates})

##
SOURCE_GROUP(level\\sprites\\.cpp FILES ${level_sprites_sources})
SOURCE_GROUP(level\\sprites\\.h FILES ${level_sprites_headers})

##
SOURCE_GROUP(level\\trees\\.cpp FILES ${level_trees_sources})
SOURCE_GROUP(level\\trees\\.h FILES ${level_trees_headers})
SOURCE_GROUP(level\\trees\\.tpp FILES ${level_trees_templates})

##
SOURCE_GROUP(level\\vis\\.cpp FILES ${level_vis_sources})
SOURCE_GROUP(level\\vis\\.h FILES ${level_vis_headers})
SOURCE_GROUP(level\\vis\\.tpp FILES ${level_vis_templates})

##
SOURCE_GROUP(materials\\.cpp FILES ${materials_sources})
SOURCE_GROUP(materials\\.h FILES ${materials_headers})

##
SOURCE_GROUP(math\\.cpp FILES ${math_sources})
SOURCE_GROUP(math\\.h FILES ${math_headers})

##
SOURCE_GROUP(math\\geom\\.cpp FILES ${math_geom_sources})
SOURCE_GROUP(math\\geom\\.h FILES ${math_geom_headers})
SOURCE_GROUP(math\\geom\\.tpp FILES ${math_geom_templates})

##
SOURCE_GROUP(math\\matrices\\.cpp FILES ${math_matrices_sources})
SOURCE_GROUP(math\\matrices\\.h FILES ${math_matrices_headers})

##
SOURCE_GROUP(math\\quaternions\\.cpp FILES ${math_quaternions_sources})
SOURCE_GROUP(math\\quaternions\\.h FILES ${math_quaternions_headers})

##
SOURCE_GROUP(math\\vectors\\.cpp FILES ${math_vectors_sources})
SOURCE_GROUP(math\\vectors\\.h FILES ${math_vectors_headers})

##
SOURCE_GROUP(ogl\\.h FILES ${ogl_headers})

##
SOURCE_GROUP(textures\\.cpp FILES ${textures_sources})
SOURCE_GROUP(textures\\.h FILES ${textures_headers})

##
SOURCE_GROUP(util\\.cpp FILES ${util_sources})
SOURCE_GROUP(util\\.h FILES ${util_headers})
SOURCE_GROUP(util\\.tpp FILES ${util_templates})

##
SOURCE_GROUP(xml\\.cpp FILES ${xml_sources})
SOURCE_GROUP(xml\\.h FILES ${xml_headers})

###################################
# Specify the include directories #
###################################

INCLUDE_DIRECTORIES(${hesperus2_SOURCE_DIR}/engine/core)

################################
# Specify the libraries to use #
################################

INCLUDE(${hesperus2_SOURCE_DIR}/UseASX.cmake)
INCLUDE(${hesperus2_SOURCE_DIR}/UseBoost.cmake)
INCLUDE(${hesperus2_SOURCE_DIR}/UseFMOD.cmake)
INCLUDE(${hesperus2_SOURCE_DIR}/UseGLEW.cmake)
INCLUDE(${hesperus2_SOURCE_DIR}/UseLodePNG.cmake)
INCLUDE(${hesperus2_SOURCE_DIR}/UsePropParser.cmake)
INCLUDE(${hesperus2_SOURCE_DIR}/UseSDL.cmake)

##########################################
# Specify the target and where to put it #
##########################################

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${hesperus2_BINARY_DIR}/lib)
ADD_LIBRARY(${targetname} STATIC ${sources} ${headers} ${templates})
INCLUDE(${hesperus2_SOURCE_DIR}/VCTargetHack.cmake)

SET_TARGET_PROPERTIES(${targetname} PROPERTIES DEBUG_OUTPUT_NAME "hesperus_d")
