// Sculpting tools for characters and creatures:
//  - tube(): a smooth, skinned sweep along a chain of bones with blended
//    weights at the joints, so elbows, knees and spines bend organically;
//  - strand(): a tapered, curved ribbon for hair, manes and tails;
//  - faceTexture(): painted anime eyes, brows and mouth on a canvas.
// All geometry comes out non-indexed with position/normal/color and
// skinIndex/skinWeight already filled in.

import * as THREE from 'three';

const _v = new THREE.Vector3(), _t = new THREE.Vector3(), _n = new THREE.Vector3(), _b = new THREE.Vector3();
const _c = new THREE.Color();

function finish(geo) {
  geo.computeVertexNormals();
  const g = geo.toNonIndexed();
  geo.dispose();
  return g;
}

// chain: [{ p: Vector3 (bind world position), bone: index }], one entry per joint.
// The segment from chain[i] to chain[i+1] belongs to chain[i].bone.
// profile(s, a) -> { rx, rz, ox?, oz? } where s in [0,1] along the chain, a = ring angle.
// color(s, a) -> hex | Color. Options: rings per segment, radial segments, blend width, caps.
export function tube(chain, profile, color, o = {}) {
  const radial = o.radial ?? 12;
  const per = o.rings ?? 6;
  const blend = o.blend ?? 0.35;
  const segs = chain.length - 1;
  const lens = [], cum = [];
  let total = 0;
  for (let i = 0; i < segs; i++) { const l = chain[i].p.distanceTo(chain[i + 1].p); cum.push(total); lens.push(l); total += l; }
  const rings = segs * per + 1;
  const pos = [], col = [], si = [], sw = [], idx = [];
  const ref = o.ref ?? new THREE.Vector3(0, 0, 1);
  const centers = [];
  for (let r = 0; r < rings; r++) {
    const seg = Math.min(segs - 1, Math.floor(r / per));
    const t = r === rings - 1 ? 1 : (r - seg * per) / per;
    const a = chain[seg].p, b = chain[seg + 1].p;
    const s = (cum[seg] + lens[seg] * t) / total;
    _t.subVectors(b, a).normalize();
    // Smooth the frame across joints by averaging with the neighbouring segment.
    if (t === 0 && seg > 0) _t.add(_v.subVectors(a, chain[seg - 1].p).normalize()).normalize();
    const center = _v.copy(a).lerp(b, t);
    centers.push(center.clone());
    let rf = Math.abs(_t.dot(ref)) > 0.9 ? new THREE.Vector3(1, 0, 0) : ref;
    _n.crossVectors(_t, rf).normalize(); // "x" axis of the ring
    _b.crossVectors(_n, _t).normalize(); // "z" axis of the ring
    if (o.flipFrame) _n.multiplyScalar(-1);
    // Bone weights: blend into the neighbours near each joint.
    const bi = chain[seg].bone;
    let w0 = 1, b1 = bi, w1 = 0;
    if (t < blend && seg > 0) { b1 = chain[seg - 1].bone; w1 = 0.5 * (1 - t / blend); }
    else if (t > 1 - blend && seg < segs - 1) { b1 = chain[seg + 1].bone; w1 = 0.5 * ((t - (1 - blend)) / blend); }
    else if (t === 1 && seg === segs - 1 && o.endBone !== undefined) { b1 = o.endBone; w1 = 0.5; }
    w0 = 1 - w1;
    for (let k = 0; k <= radial; k++) {
      const ang = (k / radial) * Math.PI * 2;
      const pr = profile(s, ang);
      const x = Math.cos(ang) * pr.rx + (pr.ox || 0), z = Math.sin(ang) * pr.rz + (pr.oz || 0);
      pos.push(center.x + _n.x * x + _b.x * z, center.y + _n.y * x + _b.y * z, center.z + _n.z * x + _b.z * z);
      const c = color(s, ang);
      _c.set(c);
      col.push(_c.r, _c.g, _c.b);
      si.push(bi, b1, 0, 0);
      sw.push(w0, w1, 0, 0);
    }
  }
  for (let r = 0; r < rings - 1; r++) {
    for (let k = 0; k < radial; k++) {
      const a = r * (radial + 1) + k, b = a + radial + 1;
      idx.push(a, a + 1, b, b, a + 1, b + 1);
    }
  }
  // Caps.
  const cap = (ring, flip) => {
    const base = ring * (radial + 1);
    const cx = [0, 0, 0];
    for (let k = 0; k < radial; k++) for (let j = 0; j < 3; j++) cx[j] += pos[(base + k) * 3 + j] / radial;
    const ci = pos.length / 3;
    pos.push(...cx);
    col.push(col[base * 3], col[base * 3 + 1], col[base * 3 + 2]);
    si.push(si[base * 4], si[base * 4 + 1], 0, 0); sw.push(sw[base * 4], sw[base * 4 + 1], 0, 0);
    for (let k = 0; k < radial; k++) flip ? idx.push(ci, base + k + 1, base + k) : idx.push(ci, base + k, base + k + 1);
  };
  if (o.capStart) cap(0, true);
  if (o.capEnd) cap(rings - 1, false);
  const g = new THREE.BufferGeometry();
  g.setAttribute('position', new THREE.Float32BufferAttribute(pos, 3));
  g.setAttribute('color', new THREE.Float32BufferAttribute(col, 3));
  g.setAttribute('skinIndex', new THREE.Uint16BufferAttribute(si, 4));
  g.setAttribute('skinWeight', new THREE.Float32BufferAttribute(sw, 4));
  g.setIndex(idx);
  orientOutward(g, centers[Math.floor(rings / 2)], Math.floor(rings / 2) * (radial + 1));
  return finish(g);
}

// Flip the triangle winding if the faces point inward (frames can be left- or right-handed).
function orientOutward(g, center, vi) {
  const P = g.attributes.position, ix = g.index.array;
  let tri = -1;
  for (let i = 0; i < ix.length; i += 3) if (ix[i] === vi) { tri = i; break; }
  if (tri < 0) return;
  const a = new THREE.Vector3().fromBufferAttribute(P, ix[tri]);
  const b = new THREE.Vector3().fromBufferAttribute(P, ix[tri + 1]);
  const c = new THREE.Vector3().fromBufferAttribute(P, ix[tri + 2]);
  const n = new THREE.Vector3().crossVectors(b.clone().sub(a), c.clone().sub(a));
  if (n.dot(a.clone().sub(center)) < 0) {
    for (let i = 0; i < ix.length; i += 3) { const t = ix[i + 1]; ix[i + 1] = ix[i + 2]; ix[i + 2] = t; }
  }
}

// A tapered ribbon along a quadratic curve. root/ctrl/tip are Vectors in bind space.
// width at root, thickness ratio, colour gradient root->tip, single bone (or two with blend).
export function strand(root, ctrl, tip, width, o = {}) {
  const segs = o.segs ?? 6, radial = o.radial ?? 5;
  const thick = o.thick ?? 0.45;
  const taper = o.taper ?? 0.92;
  const colRoot = new THREE.Color(o.root ?? 0x302630), colTip = new THREE.Color(o.tip ?? o.root ?? 0x504050);
  const shine = o.shine ? new THREE.Color(o.shine) : null;
  const center = o.center ?? new THREE.Vector3();
  const pos = [], col = [], si = [], sw = [], idx = [];
  const bone = o.bone ?? 0, bone2 = o.bone2 ?? bone;
  const p = new THREE.Vector3(), d = new THREE.Vector3(), out = new THREE.Vector3(), side = new THREE.Vector3();
  for (let i = 0; i <= segs; i++) {
    const t = i / segs, u = 1 - t;
    p.set(0, 0, 0).addScaledVector(root, u * u).addScaledVector(ctrl, 2 * u * t).addScaledVector(tip, t * t);
    d.set(0, 0, 0).addScaledVector(ctrl.clone().sub(root), 2 * u).addScaledVector(tip.clone().sub(ctrl), 2 * t).normalize();
    out.copy(p).sub(center).normalize();
    side.crossVectors(d, out).normalize();
    out.crossVectors(side, d).normalize();
    const w = width * Math.max(0.04, 1 - Math.pow(t, 1.4) * taper);
    const c = colRoot.clone().lerp(colTip, t);
    if (shine) c.lerp(shine, Math.max(0, 1 - Math.abs(t - 0.28) / 0.1) * 0.7);
    const wb = o.bone2 !== undefined ? t : 0;
    for (let k = 0; k <= radial; k++) {
      const a = (k / radial) * Math.PI * 2;
      const x = Math.cos(a) * w, y = Math.sin(a) * w * thick;
      pos.push(p.x + side.x * x + out.x * y, p.y + side.y * x + out.y * y, p.z + side.z * x + out.z * y);
      const lit = 0.88 + 0.12 * Math.max(0, Math.sin(a));
      col.push(c.r * lit, c.g * lit, c.b * lit);
      si.push(bone, bone2, 0, 0); sw.push(1 - wb, wb, 0, 0);
    }
  }
  for (let i = 0; i < segs; i++) for (let k = 0; k < radial; k++) {
    const a = i * (radial + 1) + k, b = a + radial + 1;
    idx.push(a, b, a + 1, b, b + 1, a + 1);
  }
  const g = new THREE.BufferGeometry();
  g.setAttribute('position', new THREE.Float32BufferAttribute(pos, 3));
  g.setAttribute('color', new THREE.Float32BufferAttribute(col, 3));
  g.setAttribute('skinIndex', new THREE.Uint16BufferAttribute(si, 4));
  g.setAttribute('skinWeight', new THREE.Float32BufferAttribute(sw, 4));
  g.setIndex(idx);
  // Ring 1's centre is the curve point; use it to orient faces outward.
  const t1 = 1 / segs, u1 = 1 - t1;
  const c1 = new THREE.Vector3().addScaledVector(root, u1 * u1).addScaledVector(ctrl, 2 * u1 * t1).addScaledVector(tip, t1 * t1);
  orientOutward(g, c1, radial + 1);
  return finish(g);
}

// Give a rigid (non-skinned) geometry skin attributes for one bone.
export function bindRigid(geo, bone) {
  const n = geo.attributes.position.count;
  const si = new Uint16Array(n * 4), sw = new Float32Array(n * 4);
  for (let i = 0; i < n; i++) { si[i * 4] = bone; sw[i * 4] = 1; }
  geo.setAttribute('skinIndex', new THREE.Uint16BufferAttribute(si, 4));
  geo.setAttribute('skinWeight', new THREE.Float32BufferAttribute(sw, 4));
  return geo;
}

// ---------------------------------------------------------------- faces
const hex = (c) => '#' + new THREE.Color(c).getHexString();
const shade = (c, k) => '#' + new THREE.Color(c).multiplyScalar(k).getHexString();
const tint = (c, k) => '#' + new THREE.Color(c).lerp(new THREE.Color(0xffffff), k).getHexString();

// Paint an anime-style face. Returns { open, closed } canvases (W x H).
export function paintFace(look, W = 256, H = 128) {
  const make = (closed) => {
    const cv = document.createElement('canvas');
    cv.width = W; cv.height = H;
    const g = cv.getContext('2d');
    const s = W / 256;
    g.scale(s, s);
    const fem = !!look.female;
    const eye = look.eyes ?? 0x3a6ad8;
    const brow = shade(look.hair ?? 0x302630, 0.55);
    const line = '#2a1a22';
    const ew = fem ? 23 : 21, eh = fem ? 27 : 23;
    const ey = 62, ex = 38;
    const old = look.age === 'old';
    for (const side of [-1, 1]) {
      const cx = 128 + side * ex;
      g.save();
      g.translate(cx, ey);
      if (!closed) {
        // Sclera.
        g.fillStyle = '#fbfaf7';
        g.beginPath(); g.ellipse(0, 1, ew, eh * 0.78, 0, 0, Math.PI * 2); g.fill();
        // Upper-lid shadow on the sclera.
        g.fillStyle = 'rgba(120,110,140,0.35)';
        g.beginPath(); g.ellipse(0, -eh * 0.42, ew * 0.95, eh * 0.38, 0, 0, Math.PI * 2); g.fill();
        // Iris with vertical gradient.
        const gr = g.createLinearGradient(0, -eh * 0.7, 0, eh * 0.7);
        gr.addColorStop(0, shade(eye, 0.35));
        gr.addColorStop(0.55, hex(eye));
        gr.addColorStop(1, tint(eye, 0.45));
        g.fillStyle = gr;
        g.beginPath(); g.ellipse(side * -1.5, 2, ew * 0.66, eh * 0.74, 0, 0, Math.PI * 2); g.fill();
        g.strokeStyle = shade(eye, 0.3); g.lineWidth = 1.6; g.stroke();
        // Pupil.
        g.fillStyle = shade(eye, 0.15);
        g.beginPath(); g.ellipse(side * -1.5, 1, ew * 0.26, eh * 0.36, 0, 0, Math.PI * 2); g.fill();
        // Highlights.
        g.fillStyle = '#ffffff';
        g.beginPath(); g.ellipse(side * -1.5 - 6, -7, 5.5, 7, -0.3, 0, Math.PI * 2); g.fill();
        g.globalAlpha = 0.85;
        g.beginPath(); g.ellipse(side * -1.5 + 6, 10, 2.6, 2.6, 0, 0, Math.PI * 2); g.fill();
        g.globalAlpha = 1;
        // Upper lash line with an outer flick.
        g.strokeStyle = line; g.lineCap = 'round';
        g.lineWidth = fem ? 5 : 4;
        g.beginPath();
        g.moveTo(-side * ew * 0.95, -eh * 0.35);
        g.quadraticCurveTo(0, -eh * 1.02, side * ew * 1.02, -eh * 0.45);
        g.stroke();
        if (fem) { g.lineWidth = 3; g.beginPath(); g.moveTo(side * ew * 0.95, -eh * 0.5); g.lineTo(side * (ew + 7), -eh * 0.75); g.stroke(); }
        // Lower lash hint.
        g.lineWidth = 1.5; g.globalAlpha = 0.6;
        g.beginPath(); g.moveTo(-side * ew * 0.5, eh * 0.72); g.quadraticCurveTo(0, eh * 0.86, side * ew * 0.75, eh * 0.62); g.stroke();
        g.globalAlpha = 1;
      } else {
        g.strokeStyle = line; g.lineCap = 'round'; g.lineWidth = 4;
        g.beginPath(); g.moveTo(-ew * 0.9, 2); g.quadraticCurveTo(0, 10, ew * 0.9, 2); g.stroke();
      }
      // Brow.
      g.strokeStyle = brow; g.lineCap = 'round'; g.lineWidth = fem ? 3.2 : 4.6;
      const by = -eh - (look.browRaise ?? 6);
      g.beginPath(); g.moveTo(-side * ew * 0.85, by + (look.stern ? -2 : 3)); g.quadraticCurveTo(0, by - 5, side * ew * 1.0, by + (look.stern ? 5 : 1)); g.stroke();
      if (old) { g.strokeStyle = 'rgba(120,80,70,0.35)'; g.lineWidth = 1.2; g.beginPath(); g.moveTo(side * (ew + 4), 4); g.lineTo(side * (ew + 10), 0); g.stroke(); }
      g.restore();
    }
    // Blush.
    for (const side of [-1, 1]) {
      const gr = g.createRadialGradient(128 + side * 48, 92, 1, 128 + side * 48, 92, 16);
      gr.addColorStop(0, `rgba(240,120,130,${fem ? 0.38 : 0.2})`);
      gr.addColorStop(1, 'rgba(240,120,130,0)');
      g.fillStyle = gr;
      g.fillRect(128 + side * 48 - 18, 74, 36, 36);
    }
    // Nose and mouth.
    g.strokeStyle = 'rgba(150,90,80,0.55)'; g.lineWidth = 1.6; g.lineCap = 'round';
    g.beginPath(); g.moveTo(130, 88); g.lineTo(127, 93); g.stroke();
    g.strokeStyle = '#7a3a3a'; g.lineWidth = 2.4;
    g.beginPath();
    if (look.mouth === 'frown') { g.moveTo(119, 111); g.quadraticCurveTo(128, 106, 137, 111); }
    else if (look.mouth === 'grin') { g.moveTo(116, 105); g.quadraticCurveTo(128, 116, 140, 105); }
    else { g.moveTo(120, 107); g.quadraticCurveTo(128, 111, 136, 106); }
    g.stroke();
    if (look.mouth === 'grin') { g.fillStyle = '#fff'; g.beginPath(); g.moveTo(118, 106); g.quadraticCurveTo(128, 112, 138, 106); g.closePath(); g.fill(); }
    return cv;
  };
  return { open: make(false), closed: make(true) };
}

export { hex, shade, tint };
