import faaray, sys, cv2

rj = faaray.RenderJob()
vp = faaray.ViewPlane(500, 500)
rj.setViewPlane(vp)
#rj.setOneThread()
rj.setMultiThread()
vp.setNumSamples(16)
sc = rj.getScene()


pc = faaray.PinholeCamera()
pc.setEye(0, 20, 40)
pc.setLookAt(0, 0, 0)
pc.setViewPlaneDistance(20)
pc.setZoom(1)
sc.setCamera(pc)

rct = faaray.RayCastTracer()
sc.setTracer(rct)

al = faaray.AmbientLight()
al.setLs(0.1)
sc.setAmbientLight(al)

plA = faaray.PointLight()
plA.setCenter(-10, 10, 10)
plA.setColor(1, 1, 1)
plA.setRadiance(0.7);
plA.castShadows(True)
sc.addLight(plA)

plB = faaray.PointLight()
plB.setCenter(200, 200, 200)
plB.setColor(1, 1, 1)
plB.setRadiance(0.7)
plB.castShadows(True)
sc.addLight(plB)

mmA = faaray.MatteMaterial()
mmA.setCd(1, 1, 0)
mmA.setKd(1.5)

mmB = faaray.MatteMaterial()
mmB.setCd(0.5, 0.5, 1)
mmA.setKd(1.5)

pmA = faaray.PhongMaterial()
pmA.setCd(1, 1, 0.3)
pmA.setKd(1.5)
pmA.setCs(1, 1, 1)
pmA.setKs(0.5)
pmA.setExp(20)

pmB = faaray.PhongMaterial()
pmB.setCd(0.3, 0.3, 1)
pmB.setKd(1.5)
pmB.setCs(1, 1, 1)
pmB.setKs(0.5)
pmB.setExp(20)

spA = faaray.Sphere()
spA.setCenter(5, 15, 15)
spA.setRadius(4)
spA.setMaterial(pmA)
sc.addObject(spA)

spB = faaray.Sphere()
spB.setCenter(0, 0, 0)
spB.setRadius(6)
spB.setMaterial(pmA)
sc.addObject(spB)

plA = faaray.Plane()
plA.setCenter(0, 0, 0)
plA.setNormal(0, 1, 0)
plA.setMaterial(pmA)
sc.addObject(plA)

cyA = faaray.OpenCylinder()
cyA.setCenter(0, 7, 0)
cyA.setBottom(0)
cyA.setTop(6)
cyA.setRadius(4)
cyA.setMaterial(pmB)
sc.addObject(cyA)

rj.render()

cv2.imwrite('image.png', rj.image)

print('Render Done !')

sys.exit(0)
