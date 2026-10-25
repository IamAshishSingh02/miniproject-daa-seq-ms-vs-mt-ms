// Node runtime route invokes only the repository-built benchmark executable.
import { spawn } from "node:child_process";
import { NextResponse } from "next/server";
import path from "node:path";
export const runtime = "nodejs";
const allowed = new Set(["random", "sorted", "reverse", "duplicates"]);
export async function POST(req: Request) {
  let x: any;
  try {
    x = await req.json();
  } catch {
    return NextResponse.json({ error: "Invalid JSON" }, { status: 400 });
  }
  const size = Number(x.size),
    repetitions = Number(x.repetitions ?? 3),
    distribution = String(x.distribution ?? "random"),
    seed = Number(x.seed ?? 42),
    threads = Number(x.threads ?? 0);
  if (
    !Number.isInteger(size) ||
    size < 1 ||
    size > 500000 ||
    !Number.isInteger(repetitions) ||
    repetitions < 1 ||
    repetitions > 10 ||
    !allowed.has(distribution) ||
    !Number.isInteger(seed) ||
    seed < 0 ||
    !Number.isInteger(threads) ||
    threads < 0 ||
    threads > 128
  )
    return NextResponse.json(
      { error: "Invalid benchmark configuration" },
      { status: 400 },
    );
  return new Promise<Response>((resolve) => {
    const exe = path.join(process.cwd(), "cpp/bin/mergesort-benchmark");
    const p = spawn(exe, [], { stdio: ["pipe", "pipe", "pipe"] });
    let out = "",
      err = "";
    const timer = setTimeout(() => {
      p.kill("SIGKILL");
      resolve(
        NextResponse.json({ error: "Benchmark timed out" }, { status: 504 }),
      );
    }, 30000);
    p.stdout.on("data", (d) => (out += d));
    p.stderr.on("data", (d) => (err += d));
    p.on("error", () => {
      clearTimeout(timer);
      resolve(
        NextResponse.json(
          { error: "C++ benchmark is not built. Run npm run build:cpp." },
          { status: 503 },
        ),
      );
    });
    p.on("close", (code) => {
      clearTimeout(timer);
      if (code !== 0) {
        resolve(
          NextResponse.json(
            { error: "Benchmark failed" + (err ? "" : "") },
            { status: 500 },
          ),
        );
        return;
      }
      try {
        resolve(NextResponse.json(JSON.parse(out)));
      } catch {
        resolve(
          NextResponse.json(
            { error: "Invalid benchmark output" },
            { status: 500 },
          ),
        );
      }
    });
    p.stdin.end(
      JSON.stringify({ size, repetitions, distribution, seed, threads }),
    );
  });
}
