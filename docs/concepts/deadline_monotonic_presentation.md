# Deadline Monotonic Scheduling (DMS)

## Conceptul de bază

Deadline Monotonic Scheduling este un algoritm de planificare pentru sisteme de timp real care generalizează Rate Monotonic Scheduling. Prioritatea unui task se bazează pe **deadline-ul relativ**, nu doar pe perioada sa.

### Regula fundamentală

- Taskuri cu **deadline mai scurt** → **prioritate mai înaltă**
- Taskuri cu **deadline mai lung** → **prioritate mai joasă**

### Diferență față de RMS

- RMS presupune că deadline = perioadă
- DMS permite deadline-uri **diferite de perioadă** (deadline < perioadă sau deadline > perioadă)
- DMS este mai **flexibil și general**

**Exemplu:** Un task cu perioadă 20ms dar deadline 10ms are prioritate mai mare decât unul cu perioadă 10ms dar deadline 15ms.

## Avantaje și aplicații

### De ce este util DMS?

✓ **Mai general decât RMS** - se adaptează la sisteme cu deadline-uri variabile
✓ **Optim pentru taskuri cu deadline relativ** scurt comparativ cu perioada
✓ **Flexibilitate** - permite designeri mai complex de sisteme reale
✓ **Predictibilitate** - garantează deadline-urile dacă condițiile sunt îndeplinite

### Cazuri de utilizare

- Sisteme cu taskuri care au deadline-uri diferite de perioadă
- Sisteme unde anumite taskuri necesită răspuns mai rapid decât perioada lor
- Aplicații industriale cu cerințe heterogene de timp real
- Sisteme embedded cu constrângeri stricte pe latență

**Proprietate importantă:** DMS este **optim** pentru taskuri periodice cu deadline-uri relative.

## Testul de planificabilitate

Pentru Deadline Monotonic, se folosește o versiune **mai stringentă** a testului de utilizare:

$$U = \sum_{i=1}^{n} \frac{C_i}{D_i} \leq n(2^{1/n} - 1)$$

Unde:
- **Cᵢ** = timp de execuție al taskului i
- **Dᵢ** = **deadline relativ** al taskului i (nu perioada!)
- **n** = numărul de taskuri

## Comparație RMS vs DMS

| Aspect | RMS | DMS |
|--------|-----|-----|
| Condiție | Deadline = Perioadă | Deadline ≤ Perioadă (flexibil) |
| Prioritate | Bazată pe perioadă | Bazată pe deadline |
| Complexitate | Simplu | Moderat |
| Utilizare CPU | Max ~69% | Max ~69% |
| Optimalitate | Optim pentru caz standard | Optim pentru deadline arbitrare |

### Exemplu practic

Task cu C=2ms, D=6ms, T=10ms:
- RMS: U = 2/10 = 0.20
- DMS: U = 2/6 = 0.33 (mai strict, dar mai corect)
