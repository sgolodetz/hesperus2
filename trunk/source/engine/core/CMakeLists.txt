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
SET(ai_sources
hesp/ai/AIScriptRegistrar.cpp
)

SET(ai_headers
hesp/ai/AIScriptRegistrar.h
)

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
SET(bounds_sources
hesp/bounds/AABBBounds.cpp
hesp/bounds/BoundsManager.cpp
hesp/bounds/PointBounds.cpp
hesp/bounds/SphereBounds.cpp
)

SET(bounds_headers
hesp/bounds/AABBBounds.h
hesp/bounds/Bounds.h
hesp/bounds/BoundsManager.h
hesp/bounds/PointBounds.h
hesp/bounds/SphereBounds.h
)

##
SET(brushes_sources
hesp/brushes/BrushExpander.cpp
hesp/brushes/BrushFunction.cpp
)

SET(brushes_headers
hesp/brushes/BrushExpander.h
hesp/brushes/BrushFunction.h
hesp/brushes/PolyhedralBrush.h
)

SET(brushes_templates
hesp/brushes/PolyhedralBrush.tpp
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
SET(csg_headers
hesp/csg/CSGUtil.h
)

SET(csg_templates
hesp/csg/CSGUtil.tpp
)

##
SET(database_sources hesp/database/Database.cpp)
SET(database_headers hesp/database/Database.h)
SET(database_templates hesp/database/Database.tpp)

##
SET(datastructures_sources
hesp/datastructures/FiniteStateMachine.cpp
hesp/datastructures/FSMState.cpp
hesp/datastructures/FSMTransition.cpp
hesp/datastructures/HierarchicalStateMachine.cpp
hesp/datastructures/HSMState.cpp
hesp/datastructures/HSMTransition.cpp
)

SET(datastructures_headers
hesp/datastructures/FiniteStateMachine.h
hesp/datastructures/FSMState.h
hesp/datastructures/FSMTransition.h
hesp/datastructures/HierarchicalStateMachine.h
hesp/datastructures/HSMState.h
hesp/datastructures/HSMTransition.h
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
SET(lighting_sources
hesp/lighting/Lightmap.cpp
hesp/lighting/LightmapGenerator.cpp
hesp/lighting/LightmapGrid.cpp
)

SET(lighting_headers
hesp/lighting/Light.h
hesp/lighting/Lightmap.h
hesp/lighting/LightmapGenerator.h
hesp/lighting/LightmapGrid.h
)

SET(lighting_templates
hesp/lighting/LightmapGrid.tpp
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
SET(models_sources
hesp/models/Animation.cpp
hesp/models/AnimationController.cpp
hesp/models/Bone.cpp
hesp/models/BoneHierarchy.cpp
hesp/models/BoneWeight.cpp
hesp/models/ConfiguredBone.cpp
hesp/models/ConfiguredPose.cpp
hesp/models/Mesh.cpp
hesp/models/Model.cpp
hesp/models/ModelManager.cpp
hesp/models/ModelVertex.cpp
hesp/models/Pose.cpp
hesp/models/Skeleton.cpp
hesp/models/Submesh.cpp
)

SET(models_headers
hesp/models/Animation.h
hesp/models/AnimationController.h
hesp/models/Bone.h
hesp/models/BoneHierarchy.h
hesp/models/BoneWeight.h
hesp/models/ConfiguredBone.h
hesp/models/ConfiguredPose.h
hesp/models/Mesh.h
hesp/models/Model.h
hesp/models/ModelManager.h
hesp/models/ModelVertex.h
hesp/models/Pose.h
hesp/models/PoseModifier.h
hesp/models/Skeleton.h
hesp/models/Submesh.h
)

##
SET(nav_sources
hesp/nav/AdjacencyList.cpp
hesp/nav/AdjacencyTable.cpp
hesp/nav/GlobalPathfinder.cpp
hesp/nav/NavManager.cpp
hesp/nav/NavMesh.cpp
hesp/nav/NavMeshGenerator.cpp
hesp/nav/NavMeshUtil.cpp
hesp/nav/NavPolygon.cpp
hesp/nav/PathTable.cpp
hesp/nav/PathTableGenerator.cpp
hesp/nav/StepDownLink.cpp
hesp/nav/StepLink.cpp
hesp/nav/StepUpLink.cpp
hesp/nav/WalkLink.cpp
)

SET(nav_headers
hesp/nav/AdjacencyList.h
hesp/nav/AdjacencyTable.h
hesp/nav/GlobalPathfinder.h
hesp/nav/NavDataset.h
hesp/nav/NavLink.h
hesp/nav/NavManager.h
hesp/nav/NavMesh.h
hesp/nav/NavMeshGenerator.h
hesp/nav/NavMeshUtil.h
hesp/nav/NavPolygon.h
hesp/nav/PathTable.h
hesp/nav/PathTableGenerator.h
hesp/nav/StepDownLink.h
hesp/nav/StepLink.h
hesp/nav/StepUpLink.h
hesp/nav/WalkLink.h
)

##
SET(objects_base_sources
hesp/objects/base/ComponentPropertyTypeMap.cpp
hesp/objects/base/IObjectComponent.cpp
hesp/objects/base/ListenerTable.cpp
hesp/objects/base/ObjectID.cpp
hesp/objects/base/ObjectManager.cpp
hesp/objects/base/ObjectSpecification.cpp
)

SET(objects_base_headers
hesp/objects/base/ComponentPropertyTypeMap.h
hesp/objects/base/IObjectComponent.h
hesp/objects/base/IYoke.h
hesp/objects/base/ListenerTable.h
hesp/objects/base/Message.h
hesp/objects/base/MessageHandler.h
hesp/objects/base/ObjectCommand.h
hesp/objects/base/ObjectComponent.h
hesp/objects/base/ObjectID.h
hesp/objects/base/ObjectManager.h
hesp/objects/base/ObjectSpecification.h
)

SET(objects_base_templates
hesp/objects/base/ObjectComponent.tpp
hesp/objects/base/ObjectManager.tpp
hesp/objects/base/ObjectSpecification.tpp
)

##
SET(objects_commands_sources
hesp/objects/commands/CmdBipedChangePosture.cpp
hesp/objects/commands/CmdBipedJump.cpp
hesp/objects/commands/CmdBipedMove.cpp
hesp/objects/commands/CmdBipedRun.cpp
hesp/objects/commands/CmdBipedSetLook.cpp
hesp/objects/commands/CmdBipedTurn.cpp
hesp/objects/commands/CmdBipedWalk.cpp
hesp/objects/commands/CmdUseActiveItem.cpp
)

SET(objects_commands_headers
hesp/objects/commands/CmdBipedChangePosture.h
hesp/objects/commands/CmdBipedJump.h
hesp/objects/commands/CmdBipedMove.h
hesp/objects/commands/CmdBipedRun.h
hesp/objects/commands/CmdBipedSetLook.h
hesp/objects/commands/CmdBipedTurn.h
hesp/objects/commands/CmdBipedWalk.h
hesp/objects/commands/CmdUseActiveItem.h
)

##
SET(objects_components_Activatable_sources
hesp/objects/components/CmpConsumeActivatable.cpp
hesp/objects/components/CmpItemActivatable.cpp
hesp/objects/components/ICmpActivatable.cpp
)

SET(objects_components_Activatable_headers
hesp/objects/components/CmpConsumeActivatable.h
hesp/objects/components/CmpItemActivatable.h
hesp/objects/components/ICmpActivatable.h
)

SET(objects_components_AnimChooser_sources
hesp/objects/components/CmpBipedAnimChooser.cpp
)

SET(objects_components_AnimChooser_headers
hesp/objects/components/CmpBipedAnimChooser.h
hesp/objects/components/ICmpAnimChooser.h
hesp/objects/components/ICmpBipedAnimChooser.h
)

SET(objects_components_Health_sources
hesp/objects/components/CmpHealth.cpp
)

SET(objects_components_Health_headers
hesp/objects/components/CmpHealth.h
hesp/objects/components/ICmpHealth.h
)

SET(objects_components_Inventory_sources
hesp/objects/components/CmpInventory.cpp
)

SET(objects_components_Inventory_headers
hesp/objects/components/CmpInventory.h
hesp/objects/components/ICmpInventory.h
)

SET(objects_components_Movement_sources
hesp/objects/components/CmpMovement.cpp
)

SET(objects_components_Movement_headers
hesp/objects/components/CmpMovement.h
hesp/objects/components/ICmpMovement.h
)

SET(objects_components_Orientation_sources
hesp/objects/components/CmpOrientation.cpp
)

SET(objects_components_Orientation_headers
hesp/objects/components/CmpOrientation.h
hesp/objects/components/ICmpOrientation.h
)

SET(objects_components_Ownable_sources
hesp/objects/components/CmpOwnable.cpp
)

SET(objects_components_Ownable_headers
hesp/objects/components/CmpOwnable.h
hesp/objects/components/ICmpOwnable.h
)

SET(objects_components_Projectile_sources
hesp/objects/components/CmpBasicProjectile.cpp
)

SET(objects_components_Projectile_headers
hesp/objects/components/CmpBasicProjectile.h
hesp/objects/components/ICmpProjectile.h
)

SET(objects_components_Render_sources
hesp/objects/components/CmpBasicModelRender.cpp
hesp/objects/components/CmpCharacterModelRender.cpp
hesp/objects/components/CmpModelRender.cpp
hesp/objects/components/CmpSpriteRender.cpp
)

SET(objects_components_Render_headers
hesp/objects/components/CmpBasicModelRender.h
hesp/objects/components/CmpCharacterModelRender.h
hesp/objects/components/CmpModelRender.h
hesp/objects/components/CmpSpriteRender.h
hesp/objects/components/ICmpBasicModelRender.h
hesp/objects/components/ICmpModelRender.h
hesp/objects/components/ICmpRender.h
)

SET(objects_components_Simulation_sources
hesp/objects/components/CmpPosition.cpp
hesp/objects/components/CmpSimulation.cpp
)

SET(objects_components_Simulation_headers
hesp/objects/components/CmpPosition.h
hesp/objects/components/CmpSimulation.h
hesp/objects/components/ICmpPosition.h
hesp/objects/components/ICmpSimulation.h
)

SET(objects_components_Usable_sources
hesp/objects/components/CmpProjectileWeaponUsable.cpp
hesp/objects/components/CmpScriptedUsable.cpp
hesp/objects/components/CmpUsable.cpp
)

SET(objects_components_Usable_headers
hesp/objects/components/CmpProjectileWeaponUsable.h
hesp/objects/components/CmpScriptedUsable.h
hesp/objects/components/CmpUsable.h
hesp/objects/components/ICmpAmmoNeedingUsable.h
hesp/objects/components/ICmpUsable.h
)

SET(objects_components_Yoke_sources
hesp/objects/components/CmpMinimusScriptYoke.cpp
hesp/objects/components/CmpUserBipedYoke.cpp
)

SET(objects_components_Yoke_headers
hesp/objects/components/CmpMinimusScriptYoke.h
hesp/objects/components/CmpUserBipedYoke.h
hesp/objects/components/ICmpYoke.h
)

##
SET(objects_contactresolvers_sources
hesp/objects/contactresolvers/AbsorbProjectileContactResolver.cpp
hesp/objects/contactresolvers/BounceContactResolver.cpp
hesp/objects/contactresolvers/ProjectileDamageContactResolver.cpp
)

SET(objects_contactresolvers_headers
hesp/objects/contactresolvers/AbsorbProjectileContactResolver.h
hesp/objects/contactresolvers/BounceContactResolver.h
hesp/objects/contactresolvers/ProjectileDamageContactResolver.h
)

##
SET(objects_forcegenerators_sources
hesp/objects/forcegenerators/WeightForceGenerator.cpp
)

SET(objects_forcegenerators_headers
hesp/objects/forcegenerators/WeightForceGenerator.h
)

##
SET(objects_messages_sources
hesp/objects/messages/MsgObjectDestroyed.cpp
hesp/objects/messages/MsgObjectPredestroyed.cpp
hesp/objects/messages/MsgTimeElapsed.cpp
)

SET(objects_messages_headers
hesp/objects/messages/MsgObjectDestroyed.h
hesp/objects/messages/MsgObjectPredestroyed.h
hesp/objects/messages/MsgTimeElapsed.h
)

##
SET(objects_yokes_minimus_sources
hesp/objects/yokes/minimus/MinimusGotoPositionYoke.cpp
hesp/objects/yokes/minimus/MinimusScriptYoke.cpp
)

SET(objects_yokes_minimus_headers
hesp/objects/yokes/minimus/MinimusGotoPositionYoke.h
hesp/objects/yokes/minimus/MinimusScriptYoke.h
)

##
SET(objects_yokes_user_sources
hesp/objects/yokes/user/UserBipedYoke.cpp
)

SET(objects_yokes_user_headers
hesp/objects/yokes/user/UserBipedYoke.h
)

##
SET(ogl_headers
hesp/ogl/WrappedGL.h
)

##
SET(physics_sources
hesp/physics/BoundsSupportMapping.cpp
hesp/physics/BroadPhaseCollisionDetector.cpp
hesp/physics/Contact.cpp
hesp/physics/ContactResolver.cpp
hesp/physics/ContactResolverRegistry.cpp
hesp/physics/ForceGeneratorRegistry.cpp
hesp/physics/MinkDiffSupportMapping.cpp
hesp/physics/NarrowPhaseCollisionDetector.cpp
hesp/physics/NormalPhysicsObject.cpp
hesp/physics/PhysicsObject.cpp
hesp/physics/PhysicsSystem.cpp
hesp/physics/SegmentSupportMapping.cpp
hesp/physics/SweptSupportMapping.cpp
hesp/physics/TranslatedSupportMapping.cpp
)

SET(physics_headers
hesp/physics/BoundsSupportMapping.h
hesp/physics/BroadPhaseCollisionDetector.h
hesp/physics/Contact.h
hesp/physics/ContactResolver.h
hesp/physics/ContactResolverRegistry.h
hesp/physics/ForceGenerator.h
hesp/physics/ForceGeneratorRegistry.h
hesp/physics/MinkDiffSupportMapping.h
hesp/physics/NarrowPhaseCollisionDetector.h
hesp/physics/NormalPhysicsObject.h
hesp/physics/PhysicsMaterial.h
hesp/physics/PhysicsObject.h
hesp/physics/PhysicsSystem.h
hesp/physics/SegmentSupportMapping.h
hesp/physics/SupportMapping.h
hesp/physics/SweptSupportMapping.h
hesp/physics/TranslatedSupportMapping.h
)

##
SET(portals_sources
hesp/portals/OnionPortal.cpp
hesp/portals/OnionPortalGenerator.cpp
hesp/portals/Portal.cpp
hesp/portals/PortalGenerator.cpp
)

SET(portals_headers
hesp/portals/BasePortalGenerator.h
hesp/portals/OnionPortal.h
hesp/portals/OnionPortalGenerator.h
hesp/portals/Portal.h
hesp/portals/PortalGenerator.h
)

SET(portals_templates
hesp/portals/BasePortalGenerator.tpp
)

##
SET(sprites_sources
hesp/sprites/Sprite.cpp
hesp/sprites/SpriteManager.cpp
)

SET(sprites_headers
hesp/sprites/Sprite.h
hesp/sprites/SpriteManager.h
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
SET(trees_sources
hesp/trees/BSPBranch.cpp
hesp/trees/BSPLeaf.cpp
hesp/trees/BSPNode.cpp
hesp/trees/BSPTree.cpp
hesp/trees/BSPUtil.cpp
hesp/trees/OnionBranch.cpp
hesp/trees/OnionLeaf.cpp
hesp/trees/OnionNode.cpp
hesp/trees/OnionTree.cpp
hesp/trees/OnionUtil.cpp
)

SET(trees_headers
hesp/trees/BSPBranch.h
hesp/trees/BSPCompiler.h
hesp/trees/BSPLeaf.h
hesp/trees/BSPNode.h
hesp/trees/BSPTree.h
hesp/trees/BSPUtil.h
hesp/trees/OnionBranch.h
hesp/trees/OnionCompiler.h
hesp/trees/OnionLeaf.h
hesp/trees/OnionNode.h
hesp/trees/OnionTree.h
hesp/trees/OnionUtil.h
hesp/trees/TreeUtil.h
)

SET(trees_templates
hesp/trees/BSPCompiler.tpp
hesp/trees/BSPUtil.tpp
hesp/trees/OnionCompiler.tpp
hesp/trees/TreeUtil.tpp
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
SET(vis_sources
hesp/vis/Antipenumbra.cpp
hesp/vis/VisCalculator.cpp
)

SET(vis_headers
hesp/vis/Antipenumbra.h
hesp/vis/VisCalculator.h
hesp/vis/VisTable.h
)

SET(vis_templates
hesp/vis/Antipenumbra.tpp
hesp/vis/VisTable.tpp
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
${ai_sources}
${audio_sources}
${axes_sources}
${bounds_sources}
${brushes_sources}
${cameras_sources}
${colours_sources}
${database_sources}
${datastructures_sources}
${gui_sources}
${images_sources}
${input_sources}
${io_files_sources}
${io_sections_sources}
${io_util_sources}
${level_sources}
${lighting_sources}
${materials_sources}
${math_sources}
${math_geom_sources}
${math_matrices_sources}
${math_quaternions_sources}
${math_vectors_sources}
${models_sources}
${nav_sources}
${objects_base_sources}
${objects_commands_sources}
${objects_components_Activatable_sources}
${objects_components_AnimChooser_sources}
${objects_components_Health_sources}
${objects_components_Inventory_sources}
${objects_components_Movement_sources}
${objects_components_Orientation_sources}
${objects_components_Ownable_sources}
${objects_components_Projectile_sources}
${objects_components_Render_sources}
${objects_components_Simulation_sources}
${objects_components_Usable_sources}
${objects_components_Yoke_sources}
${objects_contactresolvers_sources}
${objects_forcegenerators_sources}
${objects_messages_sources}
${objects_yokes_minimus_sources}
${objects_yokes_user_sources}
${physics_sources}
${portals_sources}
${sprites_sources}
${textures_sources}
${trees_sources}
${util_sources}
${vis_sources}
${xml_sources}
)

SET(headers
${ai_headers}
${audio_headers}
${axes_headers}
${bounds_headers}
${brushes_headers}
${cameras_headers}
${colours_headers}
${csg_headers}
${database_headers}
${datastructures_headers}
${exceptions_headers}
${gui_headers}
${images_headers}
${input_headers}
${io_files_headers}
${io_sections_headers}
${io_util_headers}
${level_headers}
${lighting_headers}
${materials_headers}
${math_headers}
${math_geom_headers}
${math_matrices_headers}
${math_quaternions_headers}
${math_vectors_headers}
${models_headers}
${nav_headers}
${objects_base_headers}
${objects_commands_headers}
${objects_components_Activatable_headers}
${objects_components_AnimChooser_headers}
${objects_components_Health_headers}
${objects_components_Inventory_headers}
${objects_components_Movement_headers}
${objects_components_Orientation_headers}
${objects_components_Ownable_headers}
${objects_components_Projectile_headers}
${objects_components_Render_headers}
${objects_components_Simulation_headers}
${objects_components_Usable_headers}
${objects_components_Yoke_headers}
${objects_contactresolvers_headers}
${objects_forcegenerators_headers}
${objects_messages_headers}
${objects_yokes_minimus_headers}
${objects_yokes_user_headers}
${ogl_headers}
${physics_headers}
${portals_headers}
${sprites_headers}
${textures_headers}
${trees_headers}
${util_headers}
${vis_headers}
${xml_headers}
)

SET(templates
${brushes_templates}
${csg_templates}
${database_templates}
${datastructures_templates}
${gui_templates}
${images_templates}
${io_files_templates}
${io_sections_templates}
${io_util_templates}
${lighting_templates}
${math_geom_templates}
${objects_base_templates}
${portals_templates}
${trees_templates}
${util_templates}
${vis_templates}
)

#############################
# Specify the source groups #
#############################

##
SOURCE_GROUP(ai\\.cpp FILES ${ai_sources})
SOURCE_GROUP(ai\\.h FILES ${ai_headers})


##
SOURCE_GROUP(audio\\.cpp FILES ${audio_sources})
SOURCE_GROUP(audio\\.h FILES ${audio_headers})

##
SOURCE_GROUP(axes\\.cpp FILES ${axes_sources})
SOURCE_GROUP(axes\\.h FILES ${axes_headers})

##
SOURCE_GROUP(bounds\\.cpp FILES ${bounds_sources})
SOURCE_GROUP(bounds\\.h FILES ${bounds_headers})

##
SOURCE_GROUP(brushes\\.cpp FILES ${brushes_sources})
SOURCE_GROUP(brushes\\.h FILES ${brushes_headers})
SOURCE_GROUP(brushes\\.tpp FILES ${brushes_templates})

##
SOURCE_GROUP(cameras\\.cpp FILES ${cameras_sources})
SOURCE_GROUP(cameras\\.h FILES ${cameras_headers})

##
SOURCE_GROUP(colours\\.cpp FILES ${colours_sources})
SOURCE_GROUP(colours\\.h FILES ${colours_headers})

##
SOURCE_GROUP(csg\\.h FILES ${csg_headers})
SOURCE_GROUP(csg\\.tpp FILES ${csg_templates})

##
SOURCE_GROUP(database\\.cpp FILES ${database_sources})
SOURCE_GROUP(database\\.h FILES ${database_headers})
SOURCE_GROUP(database\\.tpp FILES ${database_templates})

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
SOURCE_GROUP(lighting\\.cpp FILES ${lighting_sources})
SOURCE_GROUP(lighting\\.h FILES ${lighting_headers})
SOURCE_GROUP(lighting\\.tpp FILES ${lighting_templates})

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
SOURCE_GROUP(models\\.cpp FILES ${models_sources})
SOURCE_GROUP(models\\.h FILES ${models_headers})

##
SOURCE_GROUP(nav\\.cpp FILES ${nav_sources})
SOURCE_GROUP(nav\\.h FILES ${nav_headers})

##
SOURCE_GROUP(objects\\base\\.cpp FILES ${objects_base_sources})
SOURCE_GROUP(objects\\base\\.h FILES ${objects_base_headers})
SOURCE_GROUP(objects\\base\\.tpp FILES ${objects_base_templates})

##
SOURCE_GROUP(objects\\commands\\.cpp FILES ${objects_commands_sources})
SOURCE_GROUP(objects\\commands\\.h FILES ${objects_commands_headers})

##
SOURCE_GROUP(objects\\components\\Activatable\\.cpp FILES ${objects_components_Activatable_sources})
SOURCE_GROUP(objects\\components\\Activatable\\.h FILES ${objects_components_Activatable_headers})
SOURCE_GROUP(objects\\components\\AnimChooser\\.cpp FILES ${objects_components_AnimChooser_sources})
SOURCE_GROUP(objects\\components\\AnimChooser\\.h FILES ${objects_components_AnimChooser_headers})
SOURCE_GROUP(objects\\components\\Health\\.cpp FILES ${objects_components_Health_sources})
SOURCE_GROUP(objects\\components\\Health\\.h FILES ${objects_components_Health_headers})
SOURCE_GROUP(objects\\components\\Inventory\\.cpp FILES ${objects_components_Inventory_sources})
SOURCE_GROUP(objects\\components\\Inventory\\.h FILES ${objects_components_Inventory_headers})
SOURCE_GROUP(objects\\components\\Movement\\.cpp FILES ${objects_components_Movement_sources})
SOURCE_GROUP(objects\\components\\Movement\\.h FILES ${objects_components_Movement_headers})
SOURCE_GROUP(objects\\components\\Orientation\\.cpp FILES ${objects_components_Orientation_sources})
SOURCE_GROUP(objects\\components\\Orientation\\.h FILES ${objects_components_Orientation_headers})
SOURCE_GROUP(objects\\components\\Ownable\\.cpp FILES ${objects_components_Ownable_sources})
SOURCE_GROUP(objects\\components\\Ownable\\.h FILES ${objects_components_Ownable_headers})
SOURCE_GROUP(objects\\components\\Projectile\\.cpp FILES ${objects_components_Projectile_sources})
SOURCE_GROUP(objects\\components\\Projectile\\.h FILES ${objects_components_Projectile_headers})
SOURCE_GROUP(objects\\components\\Render\\.cpp FILES ${objects_components_Render_sources})
SOURCE_GROUP(objects\\components\\Render\\.h FILES ${objects_components_Render_headers})
SOURCE_GROUP(objects\\components\\Simulation\\.cpp FILES ${objects_components_Simulation_sources})
SOURCE_GROUP(objects\\components\\Simulation\\.h FILES ${objects_components_Simulation_headers})
SOURCE_GROUP(objects\\components\\Usable\\.cpp FILES ${objects_components_Usable_sources})
SOURCE_GROUP(objects\\components\\Usable\\.h FILES ${objects_components_Usable_headers})
SOURCE_GROUP(objects\\components\\Yoke\\.cpp FILES ${objects_components_Yoke_sources})
SOURCE_GROUP(objects\\components\\Yoke\\.h FILES ${objects_components_Yoke_headers})

##
SOURCE_GROUP(objects\\contactresolvers\\.cpp FILES ${objects_contactresolvers_sources})
SOURCE_GROUP(objects\\contactresolvers\\.h FILES ${objects_contactresolvers_headers})

##
SOURCE_GROUP(objects\\forcegenerators\\.cpp FILES ${objects_forcegenerators_sources})
SOURCE_GROUP(objects\\forcegenerators\\.h FILES ${objects_forcegenerators_headers})

##
SOURCE_GROUP(objects\\messages\\.cpp FILES ${objects_messages_sources})
SOURCE_GROUP(objects\\messages\\.h FILES ${objects_messages_headers})

##
SOURCE_GROUP(objects\\yokes\\minimus\\.cpp FILES ${objects_yokes_minimus_sources})
SOURCE_GROUP(objects\\yokes\\minimus\\.h FILES ${objects_yokes_minimus_headers})

##
SOURCE_GROUP(objects\\yokes\\user\\.cpp FILES ${objects_yokes_user_sources})
SOURCE_GROUP(objects\\yokes\\user\\.h FILES ${objects_yokes_user_headers})

##
SOURCE_GROUP(ogl\\.h FILES ${ogl_headers})

##
SOURCE_GROUP(physics\\.cpp FILES ${physics_sources})
SOURCE_GROUP(physics\\.h FILES ${physics_headers})

##
SOURCE_GROUP(portals\\.cpp FILES ${portals_sources})
SOURCE_GROUP(portals\\.h FILES ${portals_headers})
SOURCE_GROUP(portals\\.tpp FILES ${portals_templates})

##
SOURCE_GROUP(sprites\\.cpp FILES ${sprites_sources})
SOURCE_GROUP(sprites\\.h FILES ${sprites_headers})

##
SOURCE_GROUP(textures\\.cpp FILES ${textures_sources})
SOURCE_GROUP(textures\\.h FILES ${textures_headers})

##
SOURCE_GROUP(trees\\.cpp FILES ${trees_sources})
SOURCE_GROUP(trees\\.h FILES ${trees_headers})
SOURCE_GROUP(trees\\.tpp FILES ${trees_templates})

##
SOURCE_GROUP(util\\.cpp FILES ${util_sources})
SOURCE_GROUP(util\\.h FILES ${util_headers})
SOURCE_GROUP(util\\.tpp FILES ${util_templates})

##
SOURCE_GROUP(vis\\.cpp FILES ${vis_sources})
SOURCE_GROUP(vis\\.h FILES ${vis_headers})
SOURCE_GROUP(vis\\.tpp FILES ${vis_templates})

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
