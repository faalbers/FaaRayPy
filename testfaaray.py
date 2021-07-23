import faaray

rj = faaray.RenderJob()
vp = faaray.ViewPlane(500, 500)
rj.setViewPlane(vp)
rj.setMultiThread()
vp.setNumSamples(500)
sc = rj.getScene()

pc = faaray.PinholeCamera()
pc.setEye(0, 0, 20)
pc.setLookAt(0, 0, 20)
pc.setViewPlaneDistance(10)
pc.setZoom(10)
sc.setCamera(pc)

rct = faaray.RayCastTracer()
sc.setTracer(rct)

al = faaray.AmbientLight()
al.setLs(0.05)
sc.setAmbientLight(al);

plA = faaray.PointLight()
plA.setCenter(60, 200, 40);
plA.setColor(1, 1, 0.3);
plA.setRadiance(3);
plA.castShadows(True);
sc.addLight(plA);

plB = faaray.PointLight()
plB.setCenter(40, -40, 40);
plB.setColor(0.3, 0.3, 1);
plB.setRadiance(2);
plB.castShadows(True);
sc.addLight(plB);

mmA = faaray.MatteMaterial()
mmA.setCd(1.0, 1.0, 1.0);
mmB = faaray.MatteMaterial()
mmB.setCd(0.5, 0.5, 1.0);

spA = faaray.Sphere()
spA.setCenter(0, 0, 0);
spA.setRadius(5);
spA.setMaterial(mmB);
sc.addObject(spA);

rj.render();

print('Render Done !')
