// Third-person orbit camera with collision, lock-on framing, shake, a
// first-person mode, and scripted shots for dialogue and the title screen.

import * as THREE from 'three';
import { clamp, damp, dampAngle, lerp, wrapAngle } from '../core/util.js';

const tmp = new THREE.Vector3();

export class CameraRig {
  constructor(game) {
    this.game = game;
    this.cam = game.camera;
    this.cam.rotation.order = 'YXZ';
    this.yaw = 0; // camera south of the player, looking north
    this.pitch = -0.18;
    this.dist = 5.2;
    this.distTarget = 5.2;
    this.actualDist = 5.2;
    this.pivot = new THREE.Vector3();
    this.firstPerson = false;
    this.shakeAmt = 0;
    this.mode = 'follow'; // follow | dialogue | orbit | free
    this.orbit = { center: new THREE.Vector3(), radius: 60, height: 20, speed: 0.05, angle: 0 };
    this.focus = null;
    this.fov = 60;
  }

  shake(a) {
    if (this.game.settings.cameraShake) this.shakeAmt = Math.min(1.2, this.shakeAmt + a);
  }

  update(dt, input, player) {
    const cam = this.cam;
    if (this.mode === 'orbit') {
      const o = this.orbit;
      o.angle += dt * o.speed;
      cam.position.set(o.center.x + Math.cos(o.angle) * o.radius, o.center.y + o.height, o.center.z + Math.sin(o.angle) * o.radius);
      cam.lookAt(o.center);
      return;
    }
    if (this.mode === 'dialogue' && this.focus) {
      // Over the player's shoulder toward the speaker.
      const p = player.position, n = this.focus;
      const dx = n.x - p.x, dz = n.z - p.z;
      const d = Math.hypot(dx, dz) || 1;
      const side = new THREE.Vector3(dz / d, 0, -dx / d);
      const target = tmp.set(lerp(p.x, n.x, 0.55), Math.max(p.y, n.y) + 1.45, lerp(p.z, n.z, 0.55));
      const want = new THREE.Vector3(p.x - (dx / d) * 2.6 + side.x * 1.1, p.y + 1.8, p.z - (dz / d) * 2.6 + side.z * 1.1);
      cam.position.lerp(want, 1 - Math.exp(-6 * dt));
      const look = new THREE.Matrix4().lookAt(cam.position, target, new THREE.Vector3(0, 1, 0));
      const q = new THREE.Quaternion().setFromRotationMatrix(look);
      cam.quaternion.slerp(q, 1 - Math.exp(-6 * dt));
      this.yaw = Math.atan2(cam.position.x - target.x, cam.position.z - target.z);
      return;
    }

    // Look input.
    this.yaw -= input.lookX;
    this.pitch = clamp(this.pitch - input.lookY, -1.35, 1.1);
    if (input.zoom) this.distTarget = clamp(this.distTarget + input.zoom * 0.8, 1.6, 12);
    if (input.pressed('view')) this.firstPerson = !this.firstPerson;

    // Lock-on: swing toward the target.
    if (player.lockTarget && !player.lockTarget.dead) {
      const t = player.lockTarget.position;
      const want = Math.atan2(player.position.x - t.x, player.position.z - t.z);
      this.yaw = dampAngle(this.yaw, want, 6, dt);
      this.pitch = damp(this.pitch, -0.22, 4, dt);
    } else if (input.lastDevice === 'gamepad' || this.game.isTouch) {
      // Gentle auto-follow behind the player while moving.
      if (player.speed > 3 && Math.abs(input.lookX) < 1e-4 && !player.gliding) {
        const behind = player.yaw + Math.PI;
        this.yaw = dampAngle(this.yaw, behind, 0.6, dt);
      }
    }

    const fp = this.firstPerson;
    const headY = player.crouching ? 1.1 : 1.6;
    this.pivot.set(player.position.x, player.position.y + (fp ? headY : 1.55), player.position.z);
    if (player.swimming) this.pivot.y = Math.max(this.pivot.y, this.game.world ? 0.9 : 0);
    if (player.render) this.pivot.y += player.render.bob || 0;

    const dirX = Math.sin(this.yaw) * Math.cos(this.pitch);
    const dirY = -Math.sin(this.pitch);
    const dirZ = Math.cos(this.yaw) * Math.cos(this.pitch);
    let want = fp ? 0 : this.distTarget * (player.gliding ? 1.35 : 1) * (player.mounted ? 1.4 : 1);

    // Pull in if terrain or a collider is in the way.
    if (!fp) {
      const world = this.game.world;
      const steps = 12;
      let safe = want;
      for (let i = 1; i <= steps; i++) {
        const d = (want * i) / steps;
        const x = this.pivot.x + dirX * d, y = this.pivot.y + dirY * d, z = this.pivot.z + dirZ * d;
        const gh = this.game.inDungeon ? this.game.dungeon.floorAt(x, z) : world.heightAt(x, z);
        if (y < gh + 0.35 || (this.game.inDungeon && this.game.dungeon.solidAt(x, z, y))) { safe = Math.max(0.6, d - want / steps); break; }
      }
      if (!this.game.inDungeon) {
        const f = this.game.colliders.raycast(this.pivot.x, this.pivot.y, this.pivot.z, this.pivot.x + dirX * want, this.pivot.y + dirY * want, this.pivot.z + dirZ * want, 10);
        safe = Math.min(safe, Math.max(0.8, want * f));
      }
      want = safe;
    }
    this.actualDist = want < this.actualDist ? want : damp(this.actualDist, want, 4, dt);

    cam.position.set(this.pivot.x + dirX * this.actualDist, this.pivot.y + dirY * this.actualDist, this.pivot.z + dirZ * this.actualDist);
    if (!fp) cam.position.y = Math.max(cam.position.y, (this.game.inDungeon ? -1e9 : this.game.world.heightAt(cam.position.x, cam.position.z)) + 0.3);
    cam.rotation.set(this.pitch, this.yaw, 0);
    // Shoulder offset for a more cinematic frame.
    if (!fp) {
      const off = Math.min(0.45, this.actualDist * 0.08);
      cam.position.x += Math.cos(this.yaw) * off;
      cam.position.z += -Math.sin(this.yaw) * off;
    }

    if (this.shakeAmt > 0.001) {
      const s = this.shakeAmt * 0.12;
      cam.position.x += (Math.random() - 0.5) * s;
      cam.position.y += (Math.random() - 0.5) * s;
      cam.rotation.z += (Math.random() - 0.5) * s * 0.2;
      this.shakeAmt *= Math.exp(-8 * dt);
    }

    const wantFov = 60 + (player.sprinting ? 6 : 0) + (player.gliding ? 10 : 0);
    this.fov = damp(this.fov, wantFov, 4, dt);
    if (Math.abs(cam.fov - this.fov) > 0.05) { cam.fov = this.fov; cam.updateProjectionMatrix(); }
    player.setFirstPerson?.(fp);
  }

  // Forward direction on the ground plane (for movement relative to view).
  forward(out) { return out.set(-Math.sin(this.yaw), 0, -Math.cos(this.yaw)); }
}

export { wrapAngle };
