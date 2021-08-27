#include <iostream>
#include <vsg/all.h>
#include <vsgXchange/all.h>
#include <nastran.h>


int main(int argc, char** argv)
{
    vsg::CommandLine arguments(&argc, argv);
    //auto object = vsg::read("myimage.dds", options);

    // set up defaults and read command line arguments to override them
    auto options = vsg::Options::create();
    //options->add(vsgXchange::all::create());
    options->add(vsgXchange::nastran::create());

    vsg::ref_ptr<vsg::Node> nasFile = vsg::read_cast<vsg::Node>("truck.nas", options);

    auto windowTraits = vsg::WindowTraits::create();
    windowTraits->debugLayer = true;
    windowTraits->apiDumpLayer = false;
    arguments.read({ "--window", "-w" }, windowTraits->width, windowTraits->height);

    if (arguments.errors()) return arguments.writeErrorMessages(std::cerr);

    // set up search paths to SPIRV shaders and textures
    vsg::Paths searchPaths = vsg::getEnvPaths("VSG_FILE_PATH");


    // create the viewer and assign window(s) to it
    auto viewer = vsg::Viewer::create();

    auto window = vsg::Window::create(windowTraits);
    if (!window)
    {
        std::cout << "Could not create windows." << std::endl;
        return 1;
    }

    viewer->addWindow(window);

    // camera related details
    auto viewport = vsg::ViewportState::create(0, 0, window->extent2D().width, window->extent2D().height);
    auto perspective = vsg::Perspective::create(60.0, static_cast<double>(window->extent2D().width) / static_cast<double>(window->extent2D().height), 0.1, 10.0);
    auto lookAt = vsg::LookAt::create(vsg::dvec3(1.0, 1.0, 1.0), vsg::dvec3(0.0, 0.0, 0.0), vsg::dvec3(0.0, 0.0, 1.0));
    auto camera = vsg::Camera::create(perspective, lookAt, viewport);



    auto commandGraph = vsg::createCommandGraphForView(window, camera, nasFile);
    viewer->assignRecordAndSubmitTaskAndPresentation({ commandGraph });

    // compile the Vulkan objects
    viewer->compile();

    // assign a CloseHandler to the Viewer to respond to pressing Escape or press the window close button
    viewer->addEventHandlers({ vsg::CloseHandler::create(viewer) });
    viewer->addEventHandler(vsg::Trackball::create(camera));

    // main frame loop
    while (viewer->advanceToNextFrame())
    {
        // pass any events into EventHandlers assigned to the Viewer
        viewer->handleEvents();

        viewer->update();

        viewer->recordAndSubmit();

        viewer->present();
    }

    // clean up done automatically thanks to ref_ptr<>
    return 0;
}
