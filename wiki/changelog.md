# Changelog — EB15 Robotic Arm Project

This file documents all technical modifications, refactoring stages, bug fixes, and visual enhancements of the EB15 Robotic Arm mechatronic system.

---

- [2026-05-29] - [DOCS]: Added revisor-tcc audit report for Chapter 3 methodology with scores and technical alignment findings.
- [2026-05-29] - [DOCS]: Contextualized citation paragraphs across the written TCC chapters while preserving existing LaTeX structure, bibliography keys, and future-result placeholders.
- [2026-05-29] - [BUGFIX]: Resolved HTTP Port 8080 timeout starvation by bypassing reverse DNS lookup and optimizing GIL state lock acquisitions from 8 to 1 per simulation cycle.
- [2026-05-29] - [BUGFIX]: Fixed severe Cartesian settlement discrepancy by replacing silent path slicing IK failures with a fail-fast ValueError exception model.
- [2026-05-29] - [FEATURE]: Redesigned Webots simulation world with offline PBR jetted aluminum materials, grooved pedestal base, mechanical caps, and a polished tool flange.
