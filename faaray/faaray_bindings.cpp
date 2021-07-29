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
    auto out = std::vector<std::vector<std::vector<uint8_t>>>();
    for (GFA::Index y = 0; y < vp->height(); y++) {
        out.push_back(std::vector<std::vector<uint8_t>>());
        for (GFA::Index x = 0; x < vp->width(); x++) {
            out.back().push_back(std::vector<uint8_t>());
            out.back().back().push_back(
                (uint8_t) (std::max(std::min(vp->getPixel(x, vp->height()-y-1).b, 1.0), 0.0) * 255.0));
            out.back().back().push_back(
                (uint8_t) (std::max(std::min(vp->getPixel(x, vp->height()-y-1).g, 1.0), 0.0) * 255.0));
            out.back().back().push_back(
                (uint8_t) (std::max(std::min(vp->getPixel(x, vp->height()-y-1).r, 1.0), 0.0) * 255.0));
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
        .def_property_readonly("image", [](FaaRay::RenderJobSPtr &self) {
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
        .def("setKd", &FaaRay::MatteMaterial::setKd)
        // static_cast because it is an overloaded member function
        .def("setCd",
            static_cast<void (FaaRay::MatteMaterial::*)(const GFA::Scalar&, const GFA::Scalar&, const GFA::Scalar&)>(&FaaRay::MatteMaterial::setCd))
        ;
    
    py::class_<FaaRay::PhongMaterial, FaaRay::Material, FaaRay::PhongMaterialSPtr> ( m, "PhongMaterial" )
        .def(py::init())
        .def("setKd", &FaaRay::PhongMaterial::setKd)
        .def("setKs", &FaaRay::PhongMaterial::setKs)
        .def("setExp", &FaaRay::PhongMaterial::setExp)
        // static_cast because it is an overloaded member function
        .def("setCd",
            static_cast<void (FaaRay::PhongMaterial::*)(const GFA::Scalar&, const GFA::Scalar&, const GFA::Scalar&)>(&FaaRay::PhongMaterial::setCd))
        .def("setCs",
            static_cast<void (FaaRay::PhongMaterial::*)(const GFA::Scalar&, const GFA::Scalar&, const GFA::Scalar&)>(&FaaRay::PhongMaterial::setCs))
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

    py::class_<FaaRay::Plane, FaaRay::GeometricObject, FaaRay::PlaneSPtr> ( m, "Plane" )
        .def(py::init())
        .def("setCenter", &FaaRay::Plane::setCenter)
        .def("setNormal", &FaaRay::Plane::setNormal)
        .def("setMaterial", &FaaRay::Plane::setMaterialSPtr)
        ;
    
    py::class_<FaaRay::Disc, FaaRay::GeometricObject, FaaRay::DiscSPtr> ( m, "Disc" )
        .def(py::init())
        .def("setCenter", &FaaRay::Disc::setCenter)
        .def("setNormal", &FaaRay::Disc::setNormal)
        .def("setRadius", &FaaRay::Disc::setRadius)
        .def("setMaterial", &FaaRay::Disc::setMaterialSPtr)
        ;

    py::class_<FaaRay::OpenCylinder, FaaRay::GeometricObject, FaaRay::OpenCylinderSPtr> ( m, "OpenCylinder" )
        .def(py::init())
        .def("setCenter", &FaaRay::OpenCylinder::setCenter)
        .def("setBottom", &FaaRay::OpenCylinder::setBottom)
        .def("setTop", &FaaRay::OpenCylinder::setTop)
        .def("setRadius", &FaaRay::OpenCylinder::setRadius)
        .def("setMaterial", &FaaRay::OpenCylinder::setMaterialSPtr)
        ;

}
