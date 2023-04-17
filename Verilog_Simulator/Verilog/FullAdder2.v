module FullAdder(a, b, S, cin, cout);

  input a, b, cin;
  wire w1, w2, w3;

  output cout, S;

  and a1 (w1, a, b);
  and a2 (w2, a, cin);
  and a3 (w3, b, cin);
  or o1 (cout, w1, w2, w3);

  xor xo (S, a, b, cin);

endmodule
