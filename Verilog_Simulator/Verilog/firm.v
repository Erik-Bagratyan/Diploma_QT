module example_3_bl(out, i0, i1, i2, i3, s1, s0);
input i0, i1, i2, i3, s1, s0;
output out;
wire y0, y1, y2, y3, s1n, s0n;
not n1(s1n, s1);
not n2(s0n, s0);
and alpha(y0, i0, s1n, s0n);
and beta(y1, i1, s1n, s0);
and gamma(y2, i2, s1, s0n);
and terra(y3, i3, s1, s0);
or out2(out, y0, y1, y2, y3);
endmodule
