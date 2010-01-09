@echo off

set MapEditorSourceFiles=MapEditor/*.java MapEditor/Brushes/*.java MapEditor/BSP/*.java MapEditor/Commands/*.java MapEditor/CSG/*.java MapEditor/Event/*.java MapEditor/Geom/*.java MapEditor/Geom/Planar/*.java MapEditor/Geom/Splines/*.java MapEditor/Graphics/*.java MapEditor/GUI/*.java MapEditor/Maps/*.java MapEditor/Math/*.java MapEditor/Math/Matrices/*.java MapEditor/Math/Vectors/*.java MapEditor/Misc/*.java MapEditor/Test/*.java MapEditor/Textures/*.java
set MapEditorClassFiles=MapEditor/*.class MapEditor/Brushes/*.class MapEditor/BSP/*.class MapEditor/Commands/*.class MapEditor/CSG/*.class MapEditor/Event/*.class MapEditor/Geom/*.class MapEditor/Geom/Planar/*.class MapEditor/Geom/Splines/*.class MapEditor/Graphics/*.class MapEditor/GUI/*.class MapEditor/Maps/*.class MapEditor/Math/*.class MapEditor/Math/Matrices/*.class MapEditor/Math/Vectors/*.class MapEditor/Misc/*.class MapEditor/Test/*.class MapEditor/Textures/*.class
set MapEditorHTMLFiles=MapEditor/*.html MapEditor/Brushes/*.html MapEditor/BSP/*.html MapEditor/Commands/*.html MapEditor/CSG/*.html MapEditor/Event/*.html MapEditor/Geom/*.html MapEditor/Geom/Planar/*.html MapEditor/Geom/Splines/*.html MapEditor/Graphics/*.html MapEditor/GUI/*.html MapEditor/Maps/*.html MapEditor/Math/*.html MapEditor/Math/Matrices/*.html MapEditor/Math/Vectors/*.html MapEditor/Misc/*.html MapEditor/Test/*.html MapEditor/Textures/*.html

jar cMf MapEditor-Resources.jar Resources/*.*
cd ..
javac -cp .;libraries/jogl/lib/jogl.all.jar;libraries/jogl/lib/nativewindow.all.jar;libraries/jogl/lib/gluegen-rt.jar %MapEditorSourceFiles%
jar cmf MapEditor/Manifest.txt MapEditor/MapEditor.jar %MapEditorClassFiles%
jar cMf MapEditor/MapEditor-Source.jar %MapEditorSourceFiles% %MapEditorHTMLFiles% MapEditor/*.bat MapEditor/*.jpg MapEditor/*.txt MapEditor/MapEditor.vcproj MapEditor/MapEditor.sln MapEditor/MapEditor-Resources.jar
cd MapEditor
java VersionUpdater

set MapEditorHTMLFiles=
set MapEditorClassFiles=
set MapEditorSourceFiles=