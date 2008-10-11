png_images              = \
2_axis.png	all.png		area.png	axial.png	barh.png	bars.png \
belt.png	boxs.png	chart.png	cloudp.png	cloudq.png	colors.png \
conta.png	contfa.png	contf.png	cont.png	contt.png	cont_xyz.png \
crust.png	cutminmax2.png	cutminmax.png	densa.png	dens.png	dens_xyz.png \
dew2.png	dew.png		dots.png	drops.png	error.png	fall.png \
fish.png	fit.png		flow3.png	flow_dens.png	flow.png	legend.png \
loglog.png	map.png		mark.png	mesh_cont.png	mesh.png	mirror.png \
molecule.png	pie_chart.png	pipe2.png	pipe3.png	pipe.png	plot.png \
region.png	sample1.png	sample2.png	sample3.png	sample4.png	sample5.png \
sample6.png	sample7.png	sample8.png	sample9.png	samplea.png	sampleb.png \
samplec.png	sampled.png	samplee.png	semilog.png	several_light.png \
stem.png	step.png	surf3a.png	surf3c.png	surf3_cutoff.png \
surf3.png	surf3_rgbd_light.png		surf3_rgbd.png	surf_alpha.png	surfa.png \
surf_caxis.png	surf_cont.png	surf_cont_y.png	surfc.png	surf_cut.png	surf_fog.png \
surf.png	surf_sl.png	ternary.png	textmark.png	text.png	tile.png \
tiler.png	torus.png	tube_3d.png	tube.png	type0.png	type1.png \
type2.png	vect3.png	vectc3.png	vectc.png	vect.png

pngdir = $(docdir)/png
png_DATA = all.png $(png_images)
CLEANFILES = all.png $(png_images)
all.png: hotdogs.pts
	$(top_builddir)/examples/mgl_example
	touch all.png
	touch ../mathgl.texi

