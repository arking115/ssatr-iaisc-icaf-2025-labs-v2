# Rate Monotonic Scheduling (RMS)

## Conceptul de bază

Rate Monotonic Scheduling este un algoritm de planificare pentru sisteme de timp real cu taskuri periodice. Principiul de bază este simplu și elegant: **prioritatea unui task este invers proporțională cu perioada sa**.

### Regula fundamentală

- Taskuri cu **perioadă mai scurtă** → **prioritate mai înaltă**
- Taskuri cu **perioadă mai lungă** → **prioritate mai joasă**

**Exemplu:** Un task care se execută la fiecare 10ms are prioritate mai mare decât unul care se execută la fiecare 50ms.

## Avantaje și caracteristici

### De ce este util RMS?

✓ **Algoritm optim** pentru taskuri periodice și independente
✓ **Simplu de implementat** - nu necesită calcule complexe
✓ **Predictibil** - garantează deadline-urile pentru taskuri cu perioade cunoscute
✓ **Utilizare optimă a CPU** - până la ~69% în cazul mai multor taskuri

### Principii importante

- Se presupune că fiecare task trebuie finalizat înainte de următoarea sa perioadă (deadline = perioadă)
- Taskurile sunt independente și nu au dependențe de sincronizare
- Preempțiune: un task cu prioritate mai mare poate întrerupe executarea unuia cu prioritate mai joasă

## Testul de planificabilitate

Pentru a determina dacă un set de taskuri poate fi planificat cu succes, se folosește **testul de utilizare a CPU**:

$$U = \sum_{i=1}^{n} \frac{C_i}{T_i} \leq n(2^{1/n} - 1)$$

Unde:
- **Cᵢ** = timp de execuție al taskului i
- **Tᵢ** = perioada taskului i
- **n** = numărul de taskuri

### Interpretare

- Dacă **U ≤ n(2^(1/n) - 1)** → sistemul este cu siguranță planificabil
- Pentru **n = 2**: U ≤ 0.828 (82.8%)
- Pentru **n → ∞**: U → 0.693 (69.3%)

### Exemplu practic

2 taskuri cu C₁=1ms, T₁=4ms și C₂=1ms, T₂=5ms:
U = 1/4 + 1/5 = 0.45 < 0.828 ✓ Planificabil!
