module example_2_bl(out, a, b, c, d);
input a, b, c, d;
output out;
wire x, y;
and gate_1(x, a, b);
or gate_2(y, c, d);
xor gate_3(out, x, y);
endmodule
