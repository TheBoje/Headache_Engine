import qbs
import qbs.Process
import qbs.File
import qbs.FileInfo
import qbs.TextFile
import "../../../libs/openFrameworksCompiled/project/qtcreator/ofApp.qbs" as ofApp

Project{
    property string of_root: "../../.."

    ofApp {
        name: { return FileInfo.baseName(sourceDirectory) }

        files: [
            'src/3D/CameraManager.cpp',
            'src/3D/Model.cpp',
            'src/3D/Object3D.cpp',
            'src/Animation/Animator.cpp',
            'src/Animation/AnimatorManager.cpp',
            'src/Application.cpp',
            'src/Interface/InspectorInterface.cpp',
            'src/Interface/Interface.cpp',
            'src/Interface/Theme.cpp',
            'src/Renderer/Renderer2D.cpp',
            'src/Renderer/Renderer3D.cpp',
            'src/Shaders/Exploding/exploding.frag.glsl',
            'src/Shaders/Exploding/exploding.geom.glsl',
            'src/Shaders/Exploding/exploding.vert.glsl',
            'src/Shaders/Filters/Gaussian/gaussian.frag.glsl',
            'src/Shaders/Filters/Gaussian/gaussian.vert.glsl',
            'src/Shaders/Filters/GrayScale/grayscale.frag.glsl',
            'src/Shaders/Filters/GrayScale/grayscale.vert.glsl',
            'src/Shaders/Filters/Sobel/sobel.frag.glsl',
            'src/Shaders/Filters/Sobel/sobel.vert.glsl',
            'src/Shaders/Filters/ToneMapping/tonemapping.frag.glsl',
            'src/Shaders/Filters/ToneMapping/tonemapping.vert.glsl',
            'src/Shaders/Skybox/skybox.frag.glsl',
            'src/Shaders/Skybox/skybox.vert.glsl',
            'src/Utils/Cubemap.cpp',
            'src/Utils/Cursor.cpp',
            'src/Utils/HierarchyContainer.cpp',
            'src/Utils/ImageUtils.cpp',
            'src/Utils/Sprite.cpp',
            'src/main.cpp',
            'src/ofApp.cpp',
            'src/ofApp.h',
        ]

        of.addons: [
            'ofxImGui',
            'ofxCv',
            'ofxAssimpModelLoader',
        ]

        // additional flags for the project. the of module sets some
        // flags by default to add the core libraries, search paths...
        // this flags can be augmented through the following properties:
        of.pkgConfigs: []       // list of additional system pkgs to include
        of.includePaths: []     // include search paths
        of.cFlags: []           // flags passed to the c compiler
        of.cxxFlags: []         // flags passed to the c++ compiler
        of.linkerFlags: []      // flags passed to the linker
        of.defines: []          // defines are passed as -D to the compiler
                                // and can be checked with #ifdef or #if in the code
        of.frameworks: []       // osx only, additional frameworks to link with the project
        of.staticLibraries: []  // static libraries
        of.dynamicLibraries: [] // dynamic libraries

        // other flags can be set through the cpp module: http://doc.qt.io/qbs/cpp-module.html
        // eg: this will enable ccache when compiling
        //
        // cpp.compilerWrapper: 'ccache'

        Depends{
            name: "cpp"
        }

        // common rules that parse the include search paths, core libraries...
        Depends{
            name: "of"
        }

        // dependency with the OF library
        Depends{
            name: "openFrameworks"
        }
    }

    property bool makeOF: true  // use makfiles to compile the OF library
                                // will compile OF only once for all your projects
                                // otherwise compiled per project with qbs
    

    property bool precompileOfMain: false  // precompile ofMain.h
                                           // faster to recompile when including ofMain.h 
                                           // but might use a lot of space per project

    references: [FileInfo.joinPaths(of_root, "/libs/openFrameworksCompiled/project/qtcreator/openFrameworks.qbs")]
}
