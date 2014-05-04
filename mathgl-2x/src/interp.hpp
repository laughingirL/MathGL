//-----------------------------------------------------------------------------
template <class Treal> Treal mglLineart(const Treal *a, long nx, long ny, long nz, mreal x, mreal y, mreal z)
{
	if(!a || nx<1 || ny<1 || nz<1)	return 0;
	register long i0;
	long kx,ky,kz;
	Treal b=0,dx,dy,dz,b1,b0;
	if(x<0 || y<0 || z<0 || x>nx-1 || y>ny-1 || z>nz-1)
		return 0;
	if(nz>1 && z!=floor(z))		// 3d interpolation
	{
		kx=long(x);	ky=long(y);	kz=long(z);
		dx = x-mreal(kx);	dy = y-mreal(ky);	dz = z-mreal(kz);

		i0 = kx+nx*(ky+ny*kz);
		b0 = a[i0]*(mreal(1)-dx-dy+dx*dy) + dx*(mreal(1)-dy)*a[i0+1] +
			dy*(mreal(1)-dx)*a[i0+nx] + dx*dy*a[i0+nx+1];
		i0 = kx+nx*(ky+ny*(kz+1));
		b1 = a[i0]*(mreal(1)-dx-dy+dx*dy) + dx*(mreal(1)-dy)*a[i0+1] +
			dy*(mreal(1)-dx)*a[i0+nx] + dx*dy*a[i0+nx+1];
		b = b0 + dz*(b1-b0);
	}
	else if(ny>1 && y!=floor(y))	// 2d interpolation
	{
		kx=long(x);	ky=long(y);
		dx = x-kx;	dy=y-ky;
		i0 = kx+nx*ky;
		b = a[i0]*(mreal(1)-dx-dy+dx*dy) + dx*(mreal(1)-dy)*a[i0+1] +
			dy*(mreal(1)-dx)*a[i0+nx] + dx*dy*a[i0+nx+1];
	}
	else if(nx>1 && x!=floor(x))	// 1d interpolation
	{
		kx = long(x);
		b = a[kx] + (x-kx)*(a[kx+1]-a[kx]);
	}
	else						// no interpolation
		b = a[long(x+nx*(y+ny*z))];
	return b;
}
//-----------------------------------------------------------------------------
template <class Treal> Treal mgl_spline3t(const Treal y[4], long n, mreal dx, Treal &dy)
{
	Treal d[4], t0,t1, f0,d0,d1,f1, b[6];
	d[0] = y[1]-y[0];	//-(y[2]-4*y[1]+3*y[0])/2;
	d[1] = (y[2]-y[0])/mreal(2);
	d[2] = (y[3]-y[1])/mreal(2);
	d[3] = y[3]-y[2];	//-(y[3]-4*y[2]+3*y[1])/2;
	
	t0 = (y[2]-mreal(2)*y[1]+y[0])/mreal(2);
	t1 = (y[3]-mreal(2)*y[2]+y[1])/mreal(2);
	if(n<1)	t1=t0;
	if(n>2)	t0=t1;

	f0 = y[n];		d0 = d[n];
	f1 = y[n+1];	d1 = d[n+1];

	b[0] = f0;	b[1] = d0;	b[2] = t0;
	b[3] = mreal(10)*(f1-f0)+t1-mreal(3)*t0-mreal(4)*d1-mreal(6)*d0;
	b[4] = mreal(15)*(f0-f1)-mreal(2)*t1+mreal(3)*t0+mreal(7)*d1+mreal(8)*d0;
	b[5] = mreal(6)*(f1-f0)+t1-t0-mreal(3)*d1-mreal(3)*d0;
	dy = b[1] + dx*(mreal(2)*b[2]+dx*(mreal(3)*b[3]+dx*(mreal(4)*b[4]+dx*mreal(5)*b[5])));
//	d2y = mreal(2)*b[2] + dx*(mreal(6)*b[3]+dx*(mreal(12)*b[4]+dx*mreal(20)*b[5]));	// 2nd derivative for future
	return b[0] + dx*(b[1]+dx*(b[2]+dx*(b[3]+dx*(b[4]+dx*b[5]))));
}
//-----------------------------------------------------------------------------
template <class Treal> Treal mglSpline1t(const Treal *a, long nx, mreal x, Treal *dx=0)
{
	Treal r,d;
	if(nx>3)
	{
		register long k = long(x);
		if(k>0 && k<nx-2)	r = mgl_spline3t<Treal>(a+k-1, 1, x-k, d);
		else if(k<1)		r = mgl_spline3t<Treal>(a, 0, x, d);
		else	r = mgl_spline3t<Treal>(a+nx-4, 2, x+2-nx, d);
	}
	else if(nx<2)	{	d=0;	r = a[0];	}
	else if(nx==2)	{	d=a[1]-a[0];	r = a[0]+(a[1]-a[0])*x;	}
	else	// nx==3
	{
		Treal b1=-(a[2]-mreal(4)*a[1]+mreal(3)*a[0])/mreal(2), b2=(a[2]-mreal(2)*a[1]+a[0])/mreal(2);
		d = b1+mreal(2)*b2*x;	r = a[0]+x*(b1+b2*x);
	}
	if(dx)	*dx=d;
	return r;
}
//-----------------------------------------------------------------------------
template <class Treal> Treal mglSpline1st(const Treal *a, long nx, mreal x)
{
	Treal r,d;
	if(nx>3)
	{
		register long k = long(x);
		if(k>0 && k<nx-2)	r = mgl_spline3t<Treal>(a+k-1, 1, x-k, d);
		else if(k<1)		r = mgl_spline3t<Treal>(a, 0, x, d);
		else	r = mgl_spline3t<Treal>(a+nx-4, 2, x+2-nx, d);
	}
	else if(nx<2)	r = a[0];
	else if(nx==2)	r = a[0]+(a[1]-a[0])*x;
	else	// nx==3
	{
		Treal b1=-(a[2]-mreal(4)*a[1]+mreal(3)*a[0])/mreal(2), b2=(a[2]-mreal(2)*a[1]+a[0])/mreal(2);
		r = a[0]+x*(b1+b2*x);
	}
	return r;
}
//-----------------------------------------------------------------------------
template <class Treal> Treal mglSpline3t(const Treal *a, long nx, long ny, long nz, mreal x, mreal y, mreal z, Treal *dx=0, Treal *dy=0, Treal *dz=0)
{
	if(!a || nx<1 || ny<1 || nz<1)	return 0;
	Treal gx=0,gy=0,gz=0;
	x = x>0 ?(x<nx-1 ? x:nx-1):0;
	y = y>0 ?(y<ny-1 ? y:ny-1):0;
	z = z>0 ?(z<nz-1 ? z:nz-1):0;
	Treal b;
	if(nz>1)		// 3d interpolation
	{
		Treal tz[4], yz[4], xz[4];
		long kz = long(z), mz, k = long(y), m;
		if(kz>=nz-3)	kz = nz-4;
		kz = kz>1?kz-1:0;	mz = kz+4<nz?4:nz-kz;
		if(k>=ny-3)	k = ny-4;
		k = k>1?k-1:0;	m = k+4<ny?4:ny-k;
		for(long j=0;j<mz;j++)
		{
			Treal t[4], d[4];
			for(long i=0;i<m;i++)
				t[i] = mglSpline1t<Treal>(a+nx*(i+k+ny*(j+kz)),nx,x,d+i);
			tz[j] = mglSpline1t<Treal>(t,m,y-k,yz+j);
			xz[j] = mglSpline1t<Treal>(d,m,y-k,0);
		}
		b = mglSpline1t<Treal>(tz,mz,z-kz,&gz);
		gx = mglSpline1t<Treal>(xz,mz,z-kz,0);
		gy = mglSpline1t<Treal>(yz,mz,z-kz,0);
	}
	else if(ny>1)	// 2d interpolation
	{
		Treal t[4], d[4];
		long k = long(y), m;
		if(k>=ny-3)	k = ny-4;
		k = k>1?k-1:0;	m = k+4<ny?4:ny-k;
		for(long i=0;i<m;i++)
			t[i] = mglSpline1t<Treal>(a+nx*(i+k),nx,x,d+i);
		b = mglSpline1t<Treal>(t,m,y-k,&gy);
		gx = mglSpline1t<Treal>(d,m,y-k,0);
	}
	else	// 1d interpolation
		b = mglSpline1t<Treal>(a,nx,x,&gx);
	if(dx)	*dx=gx;	if(dy)	*dy=gy;	if(dz)	*dz=gz;
	return b;
}
//-----------------------------------------------------------------------------
template <class Treal> Treal mglSpline3st(const Treal *a, long nx, long ny, long nz, mreal x, mreal y, mreal z)
{
	if(!a || nx<1 || ny<1 || nz<1)	return 0;
	x = x>0 ?(x<nx-1 ? x:nx-1):0;
	y = y>0 ?(y<ny-1 ? y:ny-1):0;
	z = z>0 ?(z<nz-1 ? z:nz-1):0;
	Treal b;
	if(nz>1)		// 3d interpolation
	{
		Treal tz[4], t[4];
		long kz = long(z), mz, k = long(y), m;
		if(kz>=nz-3)	kz = nz-4;
		kz = kz>1?kz-1:0;	mz = kz+4<nz?4:nz-kz;
		if(k>=ny-3)	k = ny-4;
		k = k>1?k-1:0;	m = k+4<ny?4:ny-k;
		for(long j=0;j<mz;j++)
		{
			for(long i=0;i<m;i++)
				t[i] = mglSpline1t<Treal>(a+nx*(i+k+ny*(j+kz)),nx,x,0);
			tz[j] = mglSpline1t<Treal>(t,m,y-k,0);
		}
		b = mglSpline1t<Treal>(tz,mz,z-kz,0);
	}
	else if(ny>1)	// 2d interpolation
	{
		Treal t[4];
		long k = long(y), m;
		if(k>=ny-3)	k = ny-4;
		k = k>1?k-1:0;	m = k+4<ny?4:ny-k;
		for(long i=0;i<m;i++)
			t[i] = mglSpline1t<Treal>(a+nx*(i+k),nx,x,0);
		b = mglSpline1t<Treal>(t,m,y-k,0);
	}
	else	// 1d interpolation
		b = mglSpline1t<Treal>(a,nx,x,0);
	return b;
}
//-----------------------------------------------------------------------------
