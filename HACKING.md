# Repository Guidelines

## Project Structure & Module Organization
- `openbox/` contains the window manager core; focus/focus_cycle/stacking logic lives here.
- `obrender/` holds the rendering library shared by the WM and tools; `obt/` provides utility code (X11 helpers, parsing).
- `data/` ships default configs and sessions; `themes/` bundles reference themes; `doc/` contains manpage sources.
- `tools/` hosts developer utilities; `tests/` contains X11 regression scenarios; `po/` houses translations.

## Build, Test, and Development Commands
- Use Meson/Ninja only (autotools is being removed).
- First-time setup: `meson setup build` (pass feature flags like `-Dxcursor=enabled` as needed).
- Incremental builds: `meson compile -C build`.
- Tests (currently limited): `meson test -C build --print-errorlogs`. For new suites, add them to Meson to run here.
- Install into a prefix or staging dir: `meson install -C build --destdir /tmp/openbox-root`.
- For debug/ASAN builds: `meson setup build-asan -Db_sanitize=address,undefined -Dbuildtype=debug`.

## Coding Style & Naming Conventions
- C code targets `gnu99`; indent with 4 spaces, never tabs.
- Opening brace on a new line for functions; on the same line for single-line `if`/`while` conditions, otherwise move the brace to the next line.
- Always brace multi-line blocks; single-line conditionals may omit braces.
- Prefer descriptive lower_snake_case for functions/vars; constants/macros stay UPPER_SNAKE_CASE.
- Keep headers minimal and include what you use; reuse existing helper APIs in `obt/` and `obrender/` rather than duplicating logic.

## Testing Guidelines
- Existing tests are scenario-driven C fixtures under `tests/`; name new cases after the X11 behavior they cover (e.g., `fullscreen.c`, `stacking.c`).
- When adding tests, wire them into Meson’s `test()` entries so `meson test -C build` exercises them.
- For behavior changes, document manual steps (e.g., Xephyr session, `openbox --replace`) in the MR/PR description.

## Commit & Pull Request Guidelines
- Follow the convention `type(scope): summary` seen in history (e.g., `refactor(core): clean up startup notify`). Types commonly used: `fix`, `feat`, `refactor`, `chore`, `docs`.
- Commits should be focused and buildable; avoid mixing formatting with functional changes.
- Pull requests should include a short rationale, linked issue (if any), test results (`meson test` or manual notes), and before/after visuals when altering visible behavior or themes.
