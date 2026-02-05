import { Component, signal } from '@angular/core';
import { HeroComponent } from './features/hero/hero.component';

@Component({
  selector: 'app-root',
  standalone: true,
  imports: [HeroComponent], // We are using the Hero directly instead of routing for now
  templateUrl: './app.html',
  styleUrl: './app.scss'
})
export class App {
  protected readonly title = signal('feature-forge-app');
}