#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include "FaaRay.hpp"
#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

namespace py = pybind11;

// function to fetch framebuffer in RenderJob and convert to NumPy so cv2 can handle it
py::array makeImage(FaaRay::RenderJobSPtr renderJobSPtr)
{
    FaaRay::ViewPlaneSPtr vp(renderJobSPtr->getViewPlaneSPtr());
    auto out = std::vector<std::vector<uint8_t>>();
    for (GFA::Index i = 0; i < vp->width(); i++) {
        out.push_back(std::vector<uint8_t>(vp->height()));
    }
    for (GFA::Index i = 0; i < vp->width(); i++) {
        for (GFA::Index j = 0; j < vp->height(); j++) {
            out[i][j] = (uint8_t) (std::max(std::min(vp->getPixel(i, j).r, 1.0), 0.0) * 255.0);
        }
    }
    return py::cast(out);
}

PYBIND11_MODULE(faaray_bindings, m)
{
    m.doc() = "faaray ray tracing library";
    
    m.def("makeImage", makeImage);

    py::class_<FaaRay::RenderJob, FaaRay::RenderJobSPtr>( m, "RenderJob" )
        .def(py::init())
        .def("render", &FaaRay::RenderJob::render)
        .def("setViewPlane", &FaaRay::RenderJob::setViewPlaneSPtr)
        .def("getScene", &FaaRay::RenderJob::getSceneSPtr)
        .def("setMultiThread", &FaaRay::RenderJob::setMultiThread)
        .def("setOneThread", &FaaRay::RenderJob::setOneThread)
        .def("makeImage", [](FaaRay::RenderJobSPtr &self) {
                return makeImage(self);
            })
        ;
    
    py::class_<FaaRay::Scene, FaaRay::SceneSPtr> ( m, "Scene" )
        .def(py::init())
        .def("setCamera", &FaaRay::Scene::setCamera)
        .def("setTracer", &FaaRay::Scene::setTracer)
        .def("setAmbientLight", &FaaRay::Scene::setAmbientLight)
        .def("addLight", &FaaRay::Scene::addLight)
        .def("addObject", &FaaRay::Scene::addObject)
        ;

    py::class_<FaaRay::ViewPlane, FaaRay::ViewPlaneSPtr>( m, "ViewPlane" )
        .def(py::init<GFA::Size, GFA::Size>())
        .def("setNumSamples", &FaaRay::ViewPlane::setNumSamples)
        .def("numSamples", &FaaRay::ViewPlane::numSamples)
        ;
    
    // this and the additional variable to py::class_<FaaRay::PinholeCamera, ...
    // needs to be done to make sure FaaRay::Scene::setCamera works
    py::class_<FaaRay::Camera, FaaRay::CameraSPtr>(m, "Camera");

    py::class_<FaaRay::PinholeCamera, FaaRay::Camera, FaaRay::PinholeCameraSPtr> ( m, "PinholeCamera" )
        .def(py::init())
        .def("setEye",
            static_cast<void (FaaRay::PinholeCamera::*)(const GFA::Scalar&, const GFA::Scalar&, const GFA::Scalar&)>(&FaaRay::PinholeCamera::setEye))
        .def("setLookAt",
            static_cast<void (FaaRay::PinholeCamera::*)(const GFA::Scalar&, const GFA::Scalar&, const GFA::Scalar&)>(&FaaRay::PinholeCamera::setLookAt))
        .def("setViewPlaneDistance", &FaaRay::PinholeCamera::setViewPlaneDistance)
        .def("setZoom", &FaaRay::PinholeCamera::setZoom)
        ;

    // this and the additional variable to py::class_<FaaRay::RayCastTracer, ...
    // needs to be done to make sure FaaRay::Scene::setTracer works
    py::class_<FaaRay::Tracer, FaaRay::TracerSPtr>(m, "Tracer");

    py::class_<FaaRay::RayCastTracer, FaaRay::Tracer, FaaRay::RayCastTracerSPtr> ( m, "RayCastTracer" )
        .def(py::init())
        ;

    // this and the additional variable to py::class_<FaaRay::AmbientLight, ...
    // needs to be done to make sure FaaRay::Scene::setAmbientLight works
    // needs to be done to make sure FaaRay::Scene::addLight works
    py::class_<FaaRay::Light, FaaRay::LightSPtr>(m, "Light");

    py::class_<FaaRay::AmbientLight, FaaRay::Light, FaaRay::AmbientLightSPtr> ( m, "AmbientLight" )
        .def(py::init())
        .def("setLs", &FaaRay::AmbientLight::setLs)
        ;
    
    py::class_<FaaRay::PointLight, FaaRay::Light, FaaRay::PointLightSPtr> ( m, "PointLight" )
        .def(py::init())
        .def("setCenter", &FaaRay::PointLight::setCenter)
        .def("setColor",
            static_cast<void (FaaRay::PointLight::*)(const GFA::Scalar&, const GFA::Scalar&, const GFA::Scalar&)>(&FaaRay::PointLight::setColor))
        .def("setRadiance", &FaaRay::PointLight::setRadiance)
        .def("castShadows", &FaaRay::PointLight::castShadows)
        ;

    // this and the additional variable to py::class_<FaaRay::MatteMaterial, ...
    // needs to be done to make sure FaaRay::GeometricObject::setMaterial works
    py::class_<FaaRay::Material, FaaRay::MaterialSPtr>(m, "Material");

    py::class_<FaaRay::MatteMaterial, FaaRay::Material, FaaRay::MatteMaterialSPtr> ( m, "MatteMaterial" )
        .def(py::init())
        .def("setCd",
            static_cast<void (FaaRay::MatteMaterial::*)(const GFA::Scalar&, const GFA::Scalar&, const GFA::Scalar&)>(&FaaRay::MatteMaterial::setCd))
        ;
    
    // this and the additional variable to py::class_<FaaRay::Spere, ...
    // needs to be done to make sure FaaRay::Scene::addObject works
    py::class_<FaaRay::GeometricObject, FaaRay::GeometricObjectSPtr>(m, "GeometricObject");

    py::class_<FaaRay::Sphere, FaaRay::GeometricObject, FaaRay::SphereSPtr> ( m, "Sphere" )
        .def(py::init())
        .def("setCenter", &FaaRay::Sphere::setCenter)
        .def("setRadius", &FaaRay::Sphere::setRadius)
        .def("setMaterial", &FaaRay::Sphere::setMaterialSPtr)
        ;

}
