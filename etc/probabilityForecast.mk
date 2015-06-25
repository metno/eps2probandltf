

dist_pkgsysconf_DATA = \
	etc/eps2probandltf.parameters.xml \
	etc/eps2probandltf.new.parameters.xml \
	etc/eps2probandltf-1.1.parameters.xml \
	etc/felt2nc_eps_yr.xml \
	etc/damocles_felt_axes.xml \
	etc/eps2probandltf.conf \
	etc/eps_new.conf
	
# TODO: 
# Remove felt2nc_eps_yr.xml and damocles_felt_axes.xml once it is available in
# a debian package for libfimex
	
dist_pkgdataroot_DATA = \
	etc/eps2probandltf.parameters.xsd
